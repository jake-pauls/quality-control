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
    }
}

class ViewController: GLKViewController {
    private var context: EAGLContext?
    private var viewRenderer: ViewRenderer!
    
    @IBOutlet weak var upButton: UIImageView!
    @IBOutlet weak var rightButton: UIImageView!
    @IBOutlet weak var leftButton: UIImageView!
    @IBOutlet weak var downButton: UIImageView!
    
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
        
        let upButtonTap = UITapGestureRecognizer(target: self, action: #selector(ViewController.arrowTapped(gesture:)));
        upButton.addGestureRecognizer(upButtonTap);
        upButton.isUserInteractionEnabled = true;
        
        let downButtonTap = UITapGestureRecognizer(target: self, action: #selector(ViewController.arrowTapped(gesture:)));
        downButton.addGestureRecognizer(downButtonTap);
        downButton.isUserInteractionEnabled = true;
        
        let leftButtonTap = UITapGestureRecognizer(target: self, action: #selector(ViewController.arrowTapped(gesture:)));
        leftButton.addGestureRecognizer(leftButtonTap);
        leftButton.isUserInteractionEnabled = true;
        
        let rightButtonTap = UITapGestureRecognizer(target: self, action: #selector(ViewController.arrowTapped(gesture:)));
        rightButton.addGestureRecognizer(rightButtonTap);
        rightButton.isUserInteractionEnabled = true;
    }
    
    @objc func arrowTapped(gesture:UIGestureRecognizer) {
        print();
    }
    /**
     * Perform all core draw calls
     */
    override func glkView(_ view: GLKView, drawIn rect: CGRect) {
        viewRenderer.draw()
    }
}

