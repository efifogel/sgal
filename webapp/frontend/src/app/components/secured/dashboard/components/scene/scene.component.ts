import {
  Component,
  Input,
  Output,
  EventEmitter,
  AfterViewInit,
  ViewChild,
  ElementRef
} from "@angular/core";
import { PlayerNotifier } from "../../../../../services/RxJS/player.notify.service";
import { DXFJSON, Entity, Vertice } from "../../../../../models/DXF";
import { ModalService } from "../../../../../services/Modals/modals.service";

var trackball = require('../../../../../../../Three-libs/trackball.js');

declare var THREE: any;
declare var TrackballControls: any;

@Component({
  selector: "app-scene",
  templateUrl: "./scene.component.html",
  styleUrls: ["./scene.component.scss"]
})
export class SceneComponent implements AfterViewInit {
  @Input() wrlFile: any;
  @Input() title: string;
  buffer: any;
  @Output() onFileSelection: EventEmitter<string> = new EventEmitter<string>();
  @ViewChild("rendererContainer") rendererContainer: ElementRef;
  FLOOR = -250;
  
  constructor(private playerNotifier: PlayerNotifier, private modalService: ModalService) {}
  
  ngAfterViewInit() {
    const renderer = new THREE.WebGLRenderer({ antialias: true });
    const container = this.rendererContainer.nativeElement;
    const camWidth = container.offsetWidth;
    const camHeight = container.offsetHeight;
    this.playerNotifier.getSelectedFile().subscribe(file => {
      if (file.buffer) {
        this.rendererContainer.nativeElement.innerHTML = "";
        if (file.type === "json") {
          this.initJSONCanvas(file.buffer);
        }
      }
    });
  }
  
  initJSONCanvas = (data: any) => {
    let stats;
    let camera, scene, renderer, controls;
    let mouseX = 0, mouseY = 0;
    const container = this.rendererContainer.nativeElement;
    var windowHalfX = container.offsetWidth / 2;
    var windowHalfY = container.offsetHeight / 2;
    try {
      init();
      animate();
    } catch(err) {
      this.modalService.showErrorModal(`Oups, failed to parse this file: ${err}`);
    }
    function init() {
      camera = new THREE.PerspectiveCamera( 45, container.offsetWidth / container.offsetHeight, 1, 1000 );
      controls = new THREE.TrackballControls( camera, container );
      controls.rotateSpeed = 1.0;
      controls.zoomSpeed = 1.2;
      controls.panSpeed = 0.8;
      controls.noZoom = false;
      controls.noPan = false;
      controls.staticMoving = true;
      controls.dynamicDampingFactor = 0.3;
      controls.keys = [ 37, 38, 40 ];
      controls.addEventListener( 'change', render );
      // scene
      scene = new THREE.Scene();
      addSpotlight();
      var ambient = new THREE.AmbientLight( 0x444444 );
      scene.add( ambient );
      var directionalLight = new THREE.DirectionalLight( 0xffeedd );
      directionalLight.position.set( 0, 0, 1 ).normalize();
      scene.add( directionalLight );
      scene.background = "#424242";
      
      const loader = new THREE.ObjectLoader();
      const obj = loader.parse(data);
      scene.add(obj);
      
      renderer = new THREE.WebGLRenderer();
      renderer.setPixelRatio( container.devicePixelRatio );
      renderer.setSize( container.offsetWidth, container.offsetHeight );
      container.appendChild( renderer.domElement );
      const up = new THREE.Vector3();
      up.set(0, 0, 1);
      const eye = new THREE.Vector3();
      eye.set(0, -100, 40);
      const target = new THREE.Vector3();
      target.set(0, 0, 0);
      camera.up.copy(up);
      camera.position.copy(eye);
      camera.lookAt(target);
      controls.target.copy(target);
      
      // Ligths
      const light = new THREE.DirectionalLight(0xffffff, 1);
      light.position.set(0, -1, 0);
      // light.target.position.set(0, 0, 0);
      const gridHelper = new THREE.GridHelper( 10, 10 );

      camera.add(light);      
      scene.add(camera);
      scene.add(gridHelper);
      window.addEventListener( 'resize', onWindowResize, false );

    }
    function addSpotlight() {
      const spotLight = new THREE.SpotLight(0xffffff);
      spotLight.position.set(300, 300, 300);
      spotLight.intensity = 1;
      scene.add(spotLight);
    }
    
   
    function onWindowResize() {

      camera.aspect = container.offsetWidth / container.offsetHeight;
      camera.updateProjectionMatrix();
      renderer.setSize( container.offsetWidth, container.offsetHeight );
      controls.handleResize();
      render();

    }
    function onDocumentMouseMove( event ) {
      mouseX = ( event.clientX - windowHalfX ) / 2;
      mouseY = ( event.clientY - windowHalfY ) / 2;
    }
    //
    function animate() {
      controls.update();
      requestAnimationFrame( animate );
      render();
    }
    function render() {
      renderer.render( scene, camera );
    }
  }
  
}
