import {
  Component,
  Input,
  AfterViewInit,
  ViewChild,
  ElementRef
} from '@angular/core';

declare var THREE: any;

@Component({
  selector: 'app-scene',
  templateUrl: './scene.component.html',
  styleUrls: ['./scene.component.scss']
})
export class SceneComponent implements AfterViewInit {
  @Input() wrlFile: any;
  @Input() title: string;
  @ViewChild('rendererContainer') rendererContainer: ElementRef;
  constructor() {}

  ngAfterViewInit() {
    this.initCanvas();
  }

  initCanvas() {
    const canvasContainer = this.rendererContainer.nativeElement;
    let camera, controls, scene, renderer;
    init();
    animate();
    function init() {
      camera = new THREE.PerspectiveCamera(
        60,
        canvasContainer.offsetWidth / canvasContainer.offsetHeight,
        0.01,
        1e10
      );
      camera.position.z = 6;
      controls = new THREE.OrbitControls(camera);
      scene = new THREE.Scene();
      scene.add(camera);
      const dirLight = new THREE.DirectionalLight(0xffffff);
      dirLight.position.set(200, 200, 1000).normalize();
      camera.add(dirLight);
      camera.add(dirLight.target);
      const loader = new THREE.VRMLLoader();
      loader.load('assets/wrl/house.wrl', (object) => {
        scene.add(object);
      });
      renderer = new THREE.WebGLRenderer({ antialias: false });
      renderer.setPixelRatio(window.devicePixelRatio);
      renderer.setSize(canvasContainer.offsetWidth, canvasContainer.offsetHeight);
      canvasContainer.appendChild(renderer.domElement);
      window.addEventListener('resize', onWindowResize, false);
    }
    function onWindowResize() {
      camera.aspect = canvasContainer.offsetWidth / canvasContainer.offsetHeight;
      camera.updateProjectionMatrix();
      renderer.setSize(canvasContainer.offsetWidth, canvasContainer.offsetHeight);
    }
    function animate() {
      requestAnimationFrame(animate);
      renderer.render(scene, camera);
    }
  }
}
