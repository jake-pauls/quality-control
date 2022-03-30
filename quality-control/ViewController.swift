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
        
        let scoreTextContent = String(format: "Score: %d", viewRenderer.gameScore)
        let livesTextContent = String(format: "Lives: %d", viewRenderer.gameLives)
       
        let scoreTextAttrString = NSAttributedString(string: scoreTextContent, attributes: WhiteTextAttributes)
        
        let livesTextAttrString = NSAttributedString(string: livesTextContent, attributes: WhiteTextAttributes)
        
        scoreTextField.attributedText = scoreTextAttrString
        livesTextField.attributedText = livesTextAttrString
        
        // Check if game is over
        if (viewRenderer.isGameOver) {
            // Yes, this makes the font bold
            let font = UIFont(name: "Galvji", size: 30.0)!
            var traits: UIFontDescriptor.SymbolicTraits = UIFontDescriptor.SymbolicTraits()
            traits.insert(font.fontDescriptor.symbolicTraits)
            traits.insert(.traitBold)
            
            let BlackTextAttributes: [NSAttributedString.Key: Any] = [.font: UIFont(descriptor: font.fontDescriptor.withSymbolicTraits(traits)!, size: 30.0), .foregroundColor: UIColor.black]
            
            finalScoreTextField.attributedText = NSAttributedString(string: String(format: "Final Score %d", viewRenderer.gameScore), attributes: BlackTextAttributes)
            
            if lastHighScore < viewRenderer.gameScore {
                UserDefaults.standard.set(viewRenderer.gameScore, forKey: "QC_HighScore")
                
                highScoreTextField.attributedText = NSAttributedString(string: String(format: "High Score %d", viewRenderer.gameScore), attributes: BlackTextAttributes)
                lastHighScore = Int(viewRenderer.gameScore)
            } else {
                highScoreTextField.attributedText = NSAttributedString(string: String(format: "High Score %d", lastHighScore), attributes: BlackTextAttributes)
            }
            
            toggleHideGameOverMenu(false)
        }
    }
}

class ViewController: GLKViewController {
    private var context: EAGLContext?
    private var viewRenderer: ViewRenderer!
    private var lastHighScore: Int!
    
    private var AudioPlayer = AVAudioPlayer()
    
    /// Game UI Elements
    @IBOutlet weak var scoreTextField: UITextField!
    @IBOutlet weak var livesTextField: UITextField!
    
    /// Menu UI Elements
    @IBOutlet weak var titleImage: UIImageView!
    @IBOutlet weak var gameOverImage: UIImageView!
    @IBOutlet weak var restartButton: UIButton!
    @IBOutlet weak var playButton: UIButton!
    @IBOutlet weak var finalScoreTextField: UITextField!
    @IBOutlet weak var highScoreTextField: UITextField!
    
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
        let AssortedMusics = NSURL(fileURLWithPath: Bundle.main.path(forResource: "bgm", ofType: "wav")!)
       
        AudioPlayer = try! AVAudioPlayer(contentsOf: AssortedMusics as URL)
        AudioPlayer.volume = 0.05
        AudioPlayer.prepareToPlay()
        AudioPlayer.numberOfLoops = -1
        AudioPlayer.play()
        
        // TODO: Use NSAttributeStrings for styling text fields
        scoreTextField.textColor = UIColor.white
        livesTextField.textColor = UIColor.white
        
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
        
        scoreTextField.isHidden = !hide
        livesTextField.isHidden = !hide
    }
    
    func toggleHideGameOverMenu(_ hide: Bool) {
        gameOverImage.isHidden = hide
        restartButton.isHidden = hide
        finalScoreTextField.isHidden = hide
        highScoreTextField.isHidden = hide
        
        scoreTextField.isHidden = !hide
        livesTextField.isHidden = !hide
    }
    
    @IBAction func Play(_ sender: UIButton) {
        toggleHideMainMenu(true)
        viewRenderer.isGameStarted = true
    }
    
    @IBAction func Restart(_ sender: Any) {
        viewRenderer.reset()
        
        toggleHideGameOverMenu(true)
    }
}

