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
    }
    
    /**
     * Perform all core draw calls
     */
    override func glkView(_ view: GLKView, drawIn rect: CGRect) {
        viewRenderer.draw()
    }
}
