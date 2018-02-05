import {
  Component,
  Input,
  Output,
  EventEmitter,
  AfterViewInit,
  ViewChild,
  ElementRef
} from "@angular/core";
import { PlayerNotifier } from "../../../../services/RxJS/player.notify.service";
import { Geometry } from "three";
import { setTimeout } from "timers";
const fs = require("browserify-fs");

declare var THREE: any;
declare var DxfParser: any;

@Component({
  selector: "app-scene",
  templateUrl: "./scene.component.html",
  styleUrls: ["./scene.component.scss"]
})
export class SceneComponent implements AfterViewInit {
  @Input() wrlFile: any;
  @Input() title: string;
  buffer: any;
  i = 0;
  @Output() onFileSelection: EventEmitter<string> = new EventEmitter<string>();
  @ViewChild("rendererContainer") rendererContainer: ElementRef;
  constructor(private playerNotifier: PlayerNotifier) {}

  ngAfterViewInit() {
    const renderer = new THREE.WebGLRenderer({ antialias: true });
    this.playerNotifier.getSelectedFile().subscribe(buffer => {
      if (buffer.length) {
        const obj = buffer;
        const canvasContainer = this.rendererContainer.nativeElement;
        const camera = new THREE.PerspectiveCamera(
          60,
          canvasContainer.offsetWidth / canvasContainer.offsetHeight,
          0.01,
          1e10
        );
        this.i ++;
        let scene = new THREE.Scene();
        const loader = new THREE.VRMLLoader();
        scene = loader.parse(buffer);
        if (this.i < 2) {
          this.initCanvas(scene, buffer, renderer, camera, canvasContainer);
        } else {
          this.initCanvas(scene, buffer, renderer, camera, canvasContainer);
        }
      }
    });
  }

  initCanvas(scene: any, buffer: any, renderer: any, camera: any, canvasContainer: any) {
    let  controls;
    init();
    animate();
    function init() {
      camera.position.z = 6;
      controls = new THREE.OrbitControls(camera);
      scene.matrixWorldNeedsUpdate = true;
      scene.add(camera);
      const dirLight = new THREE.DirectionalLight(0xffffff);
      dirLight.position.set(200, 200, 1000).normalize();
      camera.add(dirLight);
      camera.add(dirLight.target);
      renderer.setPixelRatio(window.devicePixelRatio);
      renderer.setSize(
        canvasContainer.offsetWidth,
        canvasContainer.offsetHeight
      );
      canvasContainer.appendChild(renderer.domElement);
      window.addEventListener("resize", onWindowResize, false);
    }
    function onWindowResize() {
      camera.aspect =
      canvasContainer.offsetWidth / canvasContainer.offsetHeight;
      camera.updateProjectionMatrix();
      console.log("resizing");
      renderer.setSize(
        canvasContainer.offsetWidth,
        canvasContainer.offsetHeight
      );
    }
    function animate() {
      requestAnimationFrame(animate);
      renderer.render(scene, camera);
    }
  }
}
