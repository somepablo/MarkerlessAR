//
//  ViewController.swift
//  MarkerARExample
//
//  Created by Pablo Soto on 27/9/17.
//  Copyright © 2017 Neosentec. All rights reserved.
//

import UIKit

class ViewController: UIViewController, FrameExtractorDelegate {

    @IBOutlet weak var cameraView: UIImageView!
    @IBOutlet weak var sceneView: SCNView!
    
    let markerlessAR = MarkerlessARWrapper(UIImage(named: "onirix-markerless"))!
    let frameExtractor = FrameExtractor()
    
    var scene: SCNScene!
    var camera: SCNNode!
    var node : SCNNode!
    
    let frame = UIImage(named: "frame")
    
    override func viewDidLoad() {
        super.viewDidLoad()
        setupScene()
        frameExtractor.delegate = self
    }
    
    func onNewFrame(frame: UIImage) {
        self.cameraView.image = self.markerlessAR.processFrame(frame)
        self.camera.transform = self.markerlessAR.getCameraPose()
    }
    
    func setupScene() {
        // create a new scene
        scene = SCNScene()
        
        // create and add a camera to the scene
        camera = SCNNode()
        camera.camera = SCNCamera()
        scene.rootNode.addChildNode(camera)
        
        // place the camera
        camera.position = SCNVector3(x: 0, y: 0, z: 0)
        
        sceneView.antialiasingMode = SCNAntialiasingMode.multisampling2X
        sceneView!.scene = scene
        
        // configure the view
        sceneView!.backgroundColor = UIColor.clear
        
        if let wolfScene = SCNScene(named: "art.scnassets/wolf.dae") {
            node = wolfScene.rootNode.childNode(withName: "wolf", recursively: true)
            node.position = SCNVector3Make(0, 0, 0)
            scene.rootNode.addChildNode(node)
        }
        
        scene.rootNode.addChildNode(camera)
        
        // Set projection matrix
        let projection = markerlessAR.getCameraProjection()
        camera.camera!.projectionTransform = projection
    }
    

}

