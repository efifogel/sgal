import { Component, Input, AfterViewInit, ViewChild, ElementRef } from '@angular/core';
// import * as THREE from 'three';
import * as THREE from 'three';

@Component({
  selector: 'app-scene',
  templateUrl: './scene.component.html',
  styleUrls: ['./scene.component.scss']
})
export class SceneComponent implements AfterViewInit {

  @Input()
  wrlFile: any;
  @Input()
  title: string;
  renderer = new THREE.WebGLRenderer();
  @ViewChild('rendererContainer') rendererContainer: ElementRef;
  scene = null;
  camera = null;
  loader = null;
  mesh = null;
  constructor() {
    this.scene = new THREE.Scene();
    // this.loader = new THREE.VRMLLoader();

    this.camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 1, 10000);
    this.camera.position.z = 1000;
    const geometry = new THREE.BoxGeometry(200, 200, 200);
    const material = new THREE.MeshBasicMaterial({ color: 0xff0000, wireframe: true });
    this.mesh = new THREE.Mesh(geometry, material);
    this.scene.add(this.mesh);
  }

  ngAfterViewInit() {
   // this.renderer.setSize(window.innerWidth, window.innerHeight);
    this.rendererContainer.nativeElement.appendChild(this.renderer.domElement);
    this.animate();
  }
  animate() {
    window.requestAnimationFrame(() => this.animate());
    this.mesh.rotation.x += 0.01;
    this.mesh.rotation.y += 0.02;
    this.renderer.render(this.scene, this.camera);
  }

  initiateReader = ($img: any) => {
    if (typeof (FileReader) !== 'undefined') {
      const reader = new FileReader();
      reader.readAsArrayBuffer($img.files[0]);
    }
  }
  initiateCanvas = () => {
    
  }
}
