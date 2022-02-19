//  quality-control
//  ViewController.swift
//  2022-01-26.

import GLKit

extension ViewController: GLKViewControllerDelegate {
    func glkViewControllerUpdate(_ controller: GLKViewController) {
        // Perform rendering updates
        viewRenderer.update()
    }
}

class ViewController: GLKViewController {
    private var context: EAGLContext?
    private var viewRenderer: ViewRenderer!
    
    private func setupView() {
        context = EAGLContext(api: .openGLES3)
        EAGLContext.setCurrent(context)
        
        if let view = self.view as? GLKView, let context = context {
            view.context = context
            delegate = (self as GLKViewControllerDelegate)
            
            viewRenderer = ViewRenderer()
            viewRenderer.setup(view)
            viewRenderer.load()
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.setupView()
    }
    
    override func glkView(_ view: GLKView, drawIn rect: CGRect) {
        // Draw calls
        viewRenderer.draw()
    }
}
