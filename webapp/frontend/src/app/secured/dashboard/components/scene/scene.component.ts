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
import { Geometry, Mesh } from "three";
import { DXFJSON, Entity, Vertice } from "../../../../models/DXF";
import { ModalService } from "../../../../services/Modals/modals.service";

declare const THREE: any;
declare const DxfParser: any;
declare const ThreeDxf: any;

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
        const buffer = file.buffer;
        const camera = new THREE.PerspectiveCamera(
          40,
          container.offsetWidth / container.offsetHeight,
          1,
          100
        );
        if (file.type === "wrl") {
          this.initWRLCanvas(buffer, renderer, camera, container, false);
        } else if (file.type === "json") {
          console.log(file.buffer);
          this.initJSONCanvas(file.buffer);
        }
      }
    });
  }
  
  initJSONCanvas = (data: any) => {
    var stats;
    var camera, scene, renderer;
    var mouseX = 0, mouseY = 0;
    var windowHalfX = window.innerWidth / 2;
    var windowHalfY = window.innerHeight / 2;
    const container = this.rendererContainer.nativeElement;
    try {
      init();
      animate();
    } catch(err) {
      this.modalService.showErrorModal("Oups, failed to parse this file");
    }
    function init() {
      camera = new THREE.PerspectiveCamera( 45, container.offsetWidth / container.offsetHeight, 1, 1000 );
      // scene
      scene = new THREE.Scene();
      var ambient = new THREE.AmbientLight( 0x444444 );
      scene.add( ambient );
      var directionalLight = new THREE.DirectionalLight( 0xffeedd );
      directionalLight.position.set( 0, 0, 1 ).normalize();
      scene.add( directionalLight );

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
      // controller.target.copy(target);

      // Ligths
      const light = new THREE.DirectionalLight(0xffffff, 1);
      light.position.set(0, -1, 0);
      // light.target.position.set(0, 0, 0);
      camera.add(light);

      scene.add(camera);

      document.addEventListener( 'mousemove', onDocumentMouseMove, false );
      //
      window.addEventListener( 'resize', onWindowResize, false );
    }
    function onWindowResize() {
      windowHalfX = container.offsetWidth / 2;
      windowHalfY = container.offsetHeight / 2;
      camera.aspect = container.offsetWidth / container.offsetHeight;
      camera.updateProjectionMatrix();
      renderer.setSize( container.offsetWidth, container.offsetHeight );
    }
    function onDocumentMouseMove( event ) {
      mouseX = ( event.clientX - windowHalfX ) / 2;
      mouseY = ( event.clientY - windowHalfY ) / 2;
    }
    //
    function animate() {
      requestAnimationFrame( animate );
      render();
    }
    function render() {
     // camera.position.x += ( mouseX - camera.position.x ) * .05;
     // camera.position.y += ( mouseY - camera.position.y ) * .05;
      camera.lookAt( scene.position );
      renderer.render( scene, camera );
    }
  }
  
  parseDXF(DXF: any) {
    const parser = new DxfParser();
    try {
      const dxf = parser.parseSync(DXF);
      const geometries = [];
      dxf.entities.forEach((entity: Entity) => {
        if (entity.vertices) {
          entity.vertices.forEach((vertice: Vertice) => {
            geometries.push(vertice);
          });
        }
      });
      return { geometries, dxf };
    } catch (err) {
      console.error(err.stack);
    }
  }
  
  initWRLCanvas(
    buffer: any,
    renderer: any,
    camera: any,
    canvasContainer: any,
    isDxf: boolean
  ) {
    let controls;
    const loader = new THREE.VRMLLoader();
    const scene = loader.parse(buffer);
    
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
      var size = 10;
      var divisions = 10;
      var gridHelper = new THREE.GridHelper( size, divisions );
      scene.add( gridHelper );
      scene.background =  new THREE.Color( 0x000000 );
      window.addEventListener("resize", onWindowResize, false);
    }
    
    function onWindowResize() {
      camera.aspect =
      canvasContainer.offsetWidth / canvasContainer.offsetHeight;
      camera.updateProjectionMatrix();
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
  
  createGrid(_grid_unit) {
    const my_grid = new THREE.GridHelper(10 * _grid_unit, _grid_unit);
    my_grid.setColors("@", "@");
    return my_grid;
  }
}
