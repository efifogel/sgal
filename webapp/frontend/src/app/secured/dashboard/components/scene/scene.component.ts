import {
  Component,
  Input,
  Output,
  EventEmitter,
  AfterViewInit,
  ViewChild,
  ElementRef
} from "@angular/core";
import {PlayerNotifier} from "../../../../services/RxJS/player.notify.service";
import {Geometry} from "three";
import {DXFJSON, Entity, Vertice} from "../../../../models/DXF";

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

  constructor(private playerNotifier: PlayerNotifier) {
  }

  ngAfterViewInit() {
    const renderer = new THREE.WebGLRenderer({antialias: true});
    const container = this.rendererContainer.nativeElement;
    const camWidth = container.offsetWidth;
    const camHeight = container.offsetHeight;
    this.playerNotifier.getSelectedFile().subscribe(file => {
      if (file.buffer) {
        this.rendererContainer.nativeElement.innerHTML = "";
        const buffer = file.buffer;
        const camera = this.makeCamera(camWidth, camHeight);
        if (file.type === "wrl") {
          this.initWRLCanvas(buffer, renderer, camera, container, false);
        } else if (file.type === "json") {
          this.initJSONCanvas(file.buffer);
        }
      }
    });
  }


  initJSONCanvas = (data: any) => {
    console.log(typeof data);
    let scene, camera, pointLight;
    let renderer, mixer, animationClip;
    const clock = new THREE.Clock();
    const container = this.rendererContainer.nativeElement;
    renderer = new THREE.WebGLRenderer( { antialias: true } );
    renderer.setPixelRatio( container.devicePixelRatio );
    renderer.setSize( container.offsetWidth, container.offsetHeight );
    container.appendChild( renderer.domElement );
    scene = new THREE.Scene();
    const grid = new THREE.GridHelper( 20, 20, 0x888888, 0x888888 );
    grid.position.set( 0, - 1.1, 0 );
    scene.add( grid );
    camera = new THREE.PerspectiveCamera( 40, container.offsetWidth / container.offsetHeight, 1, 100 );
    camera.position.set( - 5.00, 3.43, 11.31 );
    camera.lookAt( new THREE.Vector3( - 1.22, 2.18, 4.58 ) );
    scene.add( new THREE.AmbientLight( 0x404040 ) );
    pointLight = new THREE.PointLight( 0xffffff, 1 );
    pointLight.position.copy( camera.position );
    scene.add( pointLight );
    const loader = new THREE.JSONLoader();

    loader.load(data, (model) => {
      scene.add(model);
    });
    /*
    new THREE.ObjectLoader().load( 'assets/JSON/pump.json', function ( model ) {
      console.log(model);
      scene.add( model );
      mixer = new THREE.AnimationMixer( model );
      mixer.clipAction( model.animations[ 0 ] ).play();
      animate();
    } );
    */
    window.onresize =  () => {
      camera.aspect = container.offsetWidth / container.offsetHeight;
      camera.updateProjectionMatrix();
      renderer.setSize( container.offsetWidth, container.offsetHeight );
    };
    function animate() {
      requestAnimationFrame( animate );
      mixer.update( clock.getDelta() );
      renderer.render( scene, camera );
    }
  }

  makeCamera(width: number, height: number) {
    return new THREE.PerspectiveCamera(60, width / height, 0.01, 1e10);
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
      return {geometries, dxf};
    } catch (err) {
      console.error(err.stack);
    }
  }



  initWRLCanvas(buffer: any,
                renderer: any,
                camera: any,
                canvasContainer: any,
                isDxf: boolean) {
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
      window.addEventListener("resize", onWindowResize, false);
    }

    function onWindowResize() {
      camera.aspect = canvasContainer.offsetWidth / canvasContainer.offsetHeight;
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


}
