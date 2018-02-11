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

  constructor(private playerNotifier: PlayerNotifier) {}

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
    const container = this.rendererContainer.nativeElement;
    const scene = new THREE.Scene();
    const camera = new THREE.PerspectiveCamera(
      75,
      container.offsetWidth / container.offsetHeight,
      0.1,
      1000
    );

    const renderer = new THREE.WebGLRenderer();
    renderer.setSize(container.innerWidth, container.innerHeight);
    container.appendChild(renderer.domElement);

    const loader = new THREE.JSONLoader();
    loader.load("assets/JSON/second.json", function(d) {
      try {
        scene.add(new Mesh(d));
      } catch (error) {
        console.log(error);
      }
      scene.add(camera);
    });

    /*let scene, camera, pointLight;
    let renderer;
    const container = this.rendererContainer.nativeElement;
    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setPixelRatio(container.devicePixelRatio);
    renderer.setSize(container.offsetWidth, container.offsetHeight);
    container.appendChild(renderer.domElement);
    scene = new THREE.Scene();
    const grid = new THREE.GridHelper(20, 20, 0x888888, 0x888888);
    grid.position.set(0, 0, 0);

    scene.add(grid);

    camera = new THREE.PerspectiveCamera(
      45,
      container.offsetWidth / container.offsetHeight,
      1,
      10
    );
    scene.add(camera);

    scene.add(new THREE.AmbientLight(0x404040));
    pointLight = new THREE.PointLight(0xffffff, 1);

    const loader = new THREE.ObjectLoader();

    loader.load('assets/JSON/pump.json', (geo) => {
      console.log(geo);
      const mesh = new Mesh(geo);
      console.log(mesh);
      scene.add(geo);
    });
   /* const FOV = 45;
    const RADIUS_SCALE = 1.1;
    const geometry = new THREE.Geometry();

    // getting the geometry object
    geometry.vertices = data.geometries[0].data.vertices;





    const mesh = new Mesh(geometry, data.materials);

    const fov_rad = FOV * Math.PI / 180.0;
    mesh.geometry.computeBoundingSphere();
    const bounding_sphere = {center: {x: 0, y: 0, z: 0}, radius: 1};
    const bbox = {min: {x: -1, y: -1, z: -1}, max: {x: 1, y: 1, z: 1}};
    const dist = bounding_sphere.radius * RADIUS_SCALE / Math.sin(fov_rad * 0.5);
    const target = new THREE.Vector3();
    const _bounding_size = new THREE.Vector3();
    mesh.geometry.computeBoundingBox();
    _bounding_size.subVectors(bbox.max, bbox.min);
    console.log(bounding_sphere, bbox);
    const half_size = new THREE.Vector3(_bounding_size.x, _bounding_size.y,
      _bounding_size.z);
    const center = new THREE.Vector3();
    center.addVectors(bbox.min, half_size);
    center.z -= half_size.z;
    console.log(mesh);


    // Translate the mesh so that the center of its projection on the Z=0
    // plane is right ar (0,0) and it is touching the Z=0 plane from bellow.
    const mat = new THREE.Matrix4();
    const v = new THREE.Vector3(0, 1, 0);
    mat.makeTranslation( -center.x, -center.y,  -center.z );
    console.log(mesh);
    mesh.geometry.applyMatrix(v);



    // Compute bounding size:

    target.set( 0, 0, half_size.z );
    const up = new THREE.Vector3();
    up.set(0, 0, 1);
    const eye = new THREE.Vector3();
    eye.set(0, -1, 0);
    eye.multiplyScalar(dist);

    eye.add(target);
    camera.up.copy(up);
    camera.position.copy(eye);
    camera.lookAt(target);

    scene.add(camera);

    scene.add(new THREE.AmbientLight(0x404040));
    pointLight = new THREE.PointLight(0xffffff, 1);


    scene.add(mesh);
    console.log(mesh);
    pointLight.position.copy(camera.position);
    scene.add(pointLight);




    window.onresize = () => {
      camera.aspect = container.offsetWidth / container.offsetHeight;
      camera.updateProjectionMatrix();
      renderer.setSize(container.offsetWidth, container.offsetHeight);
    };
    function animate() {
      requestAnimationFrame(animate);
      renderer.render(scene, camera);
    }*/
  };

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
