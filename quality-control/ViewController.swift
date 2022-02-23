//  quality-control
//  ViewController.swift
//  2022-01-26.

import GLKit

extension ViewController: GLKViewControllerDelegate {
    /**
     * Performs core rendering updates
     */
    func glkViewControllerUpdate(_ controller: GLKViewController) {
        viewRenderer.update()
        score.text = String(format:"(%)",viewRenderer.score);
    }
}

class ArrowRecognizer:UITapGestureRecognizer {
    var id = 0;
}

class ViewController: GLKViewController {
    private var context: EAGLContext?
    private var viewRenderer: ViewRenderer!
    
    @IBOutlet weak var upButton: UIImageView!
    @IBOutlet weak var rightButton: UIImageView!
    @IBOutlet weak var leftButton: UIImageView!
    @IBOutlet weak var downButton: UIImageView!
    
    @IBOutlet weak var score: UITextField!
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
            viewRenderer.awake()
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.setupView()
        
        /*
         Tapgesture Recognizer for up button
         id = 1
         */
        let upButtonTap = ArrowRecognizer(target: self, action: #selector(ViewController.arrowTapped(gesture:)));
        upButtonTap.id = 1;
        upButton.addGestureRecognizer(upButtonTap);
        upButton.isUserInteractionEnabled = true;
        
        /*
         Tapgesture Recognizer for right button
         id = 2
         */
        let rightButtonTap = ArrowRecognizer(target: self, action: #selector(ViewController.arrowTapped(gesture:)));
        rightButtonTap.id = 2;
        rightButton.addGestureRecognizer(rightButtonTap);
        rightButton.isUserInteractionEnabled = true;
        
        /*
         Tapgesture Recognizer for down button
         id = 3
         */
        let downButtonTap = ArrowRecognizer(target: self, action: #selector(ViewController.arrowTapped(gesture:)));
        downButtonTap.id = 3;
        downButton.addGestureRecognizer(downButtonTap);
        downButton.isUserInteractionEnabled = true;
        
        /*
         Tapgesture Recognizer for left button
         id = 4
         */
        let leftButtonTap = ArrowRecognizer(target: self, action: #selector(ViewController.arrowTapped(gesture:)));
        leftButtonTap.id = 4;
        leftButton.addGestureRecognizer(leftButtonTap);
        leftButton.isUserInteractionEnabled = true;
    }
    
    @objc func arrowTapped(gesture:ArrowRecognizer) {
        if (gesture.view as? UIImageView) != nil {
            switch gesture.id {
            case 1:
                print("Up");
            case 2:
                print("Right");
            case 3:
                print("Down");
            case 4:
                print("Left");
            default:
                print("Default");
               
            }
        }
    }
    /**
     * Perform all core draw calls
     */
    override func glkView(_ view: GLKView, drawIn rect: CGRect) {
        viewRenderer.draw()
    }
}

