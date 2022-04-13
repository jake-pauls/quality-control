//  quality-control
//  ViewController.swift
//  2022-01-26.

import GLKit
import AVFoundation

let WhiteTextAttributes: [NSAttributedString.Key: Any] = [.font: UIFont(name: "Galvji", size: 24.0)!, .foregroundColor: UIColor.white]


extension ViewController: GLKViewControllerDelegate {
    /**
     * Performs core rendering updates
     */
    func glkViewControllerUpdate(_ controller: GLKViewController) {
        viewRenderer.update()
        
        // Update heart icons for lives
        if viewRenderer.isGameStarted {
            updateHeartIcons(numberOfLives: Int(viewRenderer.gameLives))
        }
        
        let scoreTextContent = String(format: "SCORE: %d", viewRenderer.gameScore)
        
        let scoreTextAttrString = NSAttributedString(string: scoreTextContent, attributes: WhiteTextAttributes)
        
        scoreTextField.attributedText = scoreTextAttrString
        
        // Check if game is over
        if (viewRenderer.isGameOver) {
            // Yes, this makes the font bold
            let font = UIFont(name: "Galvji", size: 30.0)!
            var traits: UIFontDescriptor.SymbolicTraits = UIFontDescriptor.SymbolicTraits()
            traits.insert(font.fontDescriptor.symbolicTraits)
            traits.insert(.traitBold)
            
            let BlackTextAttributes: [NSAttributedString.Key: Any] = [.font: UIFont(descriptor: font.fontDescriptor.withSymbolicTraits(traits)!, size: 30.0), .foregroundColor: UIColor.black]
            
            finalScoreTextField.attributedText = NSAttributedString(string: String(format: "Final Score: %d", viewRenderer.gameScore), attributes: BlackTextAttributes)
            
            if lastHighScore < viewRenderer.gameScore {
                UserDefaults.standard.set(viewRenderer.gameScore, forKey: "QC_HighScore")
                
                highScoreTextField.attributedText = NSAttributedString(string: String(format: "High Score: %d", viewRenderer.gameScore), attributes: BlackTextAttributes)
                lastHighScore = Int(viewRenderer.gameScore)
            } else {
                highScoreTextField.attributedText = NSAttributedString(string: String(format: "High Score: %d", lastHighScore), attributes: BlackTextAttributes)
            }
            
            toggleHideGameOverMenu(false)
        }
    }
}

class ViewController: GLKViewController {
    private var context: EAGLContext?
    private var viewRenderer: ViewRenderer!
    private var lastHighScore: Int!
    
    private var MusicPlayer = AVAudioPlayer()
    private var SfxPlayer = AVAudioPlayer()
    
    /// Game UI Elements
    @IBOutlet weak var scoreTextField: UITextField!
    
    /// Menu UI Elements
    @IBOutlet weak var titleImage: UIImageView!
    @IBOutlet weak var gameOverImage: UIImageView!
    @IBOutlet weak var restartButton: UIButton!
    @IBOutlet weak var playButton: UIButton!
    @IBOutlet weak var finalScoreTextField: UITextField!
    @IBOutlet weak var highScoreTextField: UITextField!
    
    /// UI Heart Icons
    @IBOutlet weak var singleHeartIcon: UIImageView!
    @IBOutlet weak var doubleHeartIcon: UIImageView!
    @IBOutlet weak var tripleHeartIcon: UIImageView!
    
    /**
     * Initializes the GL view from a Swift context
     * Awakens and initializes the game state
     */
    private func setupView() {
        context = EAGLContext(api: .openGLES3)
        EAGLContext.setCurrent(context)
        
        if let view = self.view as? GLKView, let context = context {
            view.context = context
            delegate = (self as GLKViewControllerDelegate)
            
            viewRenderer = ViewRenderer()
            viewRenderer.setup(view)
            viewRenderer.loadModels()
            viewRenderer.awake()
        }
    }
    
    /**
     * Perform all core draw calls
     */
    override func glkView(_ view: GLKView, drawIn rect: CGRect) {
        viewRenderer.draw()
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.setupView()
        
        // Hide gameplay buttons (or gestures)
        gameOverImage.isHidden = true
        restartButton.isHidden = true
        finalScoreTextField.isHidden = true
        highScoreTextField.isHidden = true
        toggleHideMainMenu(false)
        
        // Set default gameplay booleans
        viewRenderer.isGameStarted = false
        viewRenderer.isGameOver = false
        
        // Retrieve last high score value
        lastHighScore = UserDefaults.standard.integer(forKey: "QC_HighScore")
        
        // Setup audio
        let BGM = NSURL(fileURLWithPath: Bundle.main.path(forResource: "bgm", ofType: "wav")!)
       
        MusicPlayer = try! AVAudioPlayer(contentsOf: BGM as URL)
        MusicPlayer.volume = 0.05
        MusicPlayer.prepareToPlay()
        MusicPlayer.numberOfLoops = -1
        MusicPlayer.play()
        
        let buttonSFX = NSURL(fileURLWithPath: Bundle.main.path(forResource: "buttonClick", ofType: "wav")!)
        
        SfxPlayer = try! AVAudioPlayer(contentsOf: buttonSFX as URL)
        SfxPlayer.volume = 1
        SfxPlayer.prepareToPlay()
        SfxPlayer.numberOfLoops = 0
        
        // TODO: Use NSAttributeStrings for styling text fields
        scoreTextField.textColor = UIColor.white
        
        let swipeRight = UISwipeGestureRecognizer(target: self, action: #selector(self.doSwipes(_:)))
        swipeRight.direction = .right
        view.addGestureRecognizer(swipeRight)
        
        let swipeLeft = UISwipeGestureRecognizer(target: self, action: #selector(self.doSwipes(_:)))
        swipeLeft.direction = .left
        view.addGestureRecognizer(swipeLeft)
        
        let swipeDown = UISwipeGestureRecognizer(target: self, action: #selector(self.doSwipes(_:)))
        swipeDown.direction = .down
        view.addGestureRecognizer(swipeDown)
        
        let swipeUp = UISwipeGestureRecognizer(target: self, action: #selector(self.doSwipes(_:)))
        swipeUp.direction = .up
        view.addGestureRecognizer(swipeUp)
    }
    
    @objc func doSwipes(_ gesture: UIGestureRecognizer) {
        if viewRenderer.isGameStarted {
            if let swipeGesture = gesture as? UISwipeGestureRecognizer {
                switch swipeGesture.direction {
                case .right:
                    viewRenderer.handleInput(1)
                    break
                case .left:
                    viewRenderer.handleInput(3)
                    break
                case .down:
                    viewRenderer.handleInput(2)
                    break
                case .up:
                    viewRenderer.handleInput(0)
                    break
                default:
                    break
                }
            }
        }
    }
    
    func toggleHideMainMenu(_ hide: Bool) {
        playButton.isHidden = hide
        titleImage.isHidden = hide
        
        singleHeartIcon.isHidden = !hide
        doubleHeartIcon.isHidden = !hide
        tripleHeartIcon.isHidden = !hide
        scoreTextField.isHidden = !hide
    }
    
    func toggleHideGameOverMenu(_ hide: Bool) {
        gameOverImage.isHidden = hide
        restartButton.isHidden = hide
        finalScoreTextField.isHidden = hide
        highScoreTextField.isHidden = hide
        
        singleHeartIcon.isHidden = !hide
        doubleHeartIcon.isHidden = !hide
        tripleHeartIcon.isHidden = !hide
        scoreTextField.isHidden = !hide
    }
    
    @IBAction func Play(_ sender: UIButton) {
        toggleHideMainMenu(true)
        SfxPlayer.play()
        viewRenderer.isGameStarted = true
    }
    
    @IBAction func Restart(_ sender: Any) {
        viewRenderer.reset()
        SfxPlayer.play()
        toggleHideGameOverMenu(true)
    }
    
    func updateHeartIcons(numberOfLives: Int) {
        switch(numberOfLives) {
        case 3:
            singleHeartIcon.isHidden = true
            doubleHeartIcon.isHidden = true
            tripleHeartIcon.isHidden = false
            break
        case 2:
            singleHeartIcon.isHidden = true
            doubleHeartIcon.isHidden = false
            tripleHeartIcon.isHidden = true
            break
        case 1:
            singleHeartIcon.isHidden = false
            doubleHeartIcon.isHidden = true
            tripleHeartIcon.isHidden = true
            break
        default:
            singleHeartIcon.isHidden = true
            doubleHeartIcon.isHidden = true
            tripleHeartIcon.isHidden = true
            break
        }
    }
}

