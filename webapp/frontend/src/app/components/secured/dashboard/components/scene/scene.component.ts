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


      let radiusScale = 1.1;


      scene = new THREE.Scene();

      // camera
      const start_position = new THREE.Vector3();
      const start_target = new THREE.Vector3();
      const start_up = new THREE.Vector3();
      const start_quaternion = new THREE.Quaternion();
      const start_animation = false;
      var aspect = container.offsetWidth / container.offsetHeight;
      camera = new THREE.PerspectiveCamera(45, aspect, 1, 10000);
      scene.add(camera);
      let up = new THREE.Vector3();
      up.set(0, 0, 1);
      let eye = new THREE.Vector3();
      eye.set(0, -1000, 100);
      let target = new THREE.Vector3();
      target.set(0, 0, 0);
      camera.up.copy(up);
      camera.position.copy(eye);
      camera.lookAt(target);      
      // Ligths
      const light = new THREE.DirectionalLight(0xffffff, 1);
      light.position.set(0, -1, 0);
      // light.target.position.set(0, 0, 0);
      camera.add(light);
      // control trackball
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
      addSpotlight();
      var ambient = new THREE.AmbientLight( 0x444444 );
      scene.add( ambient );
      var directionalLight = new THREE.DirectionalLight( 0xffeedd );
      directionalLight.position.set( 0, 0, 1 ).normalize();
      scene.add( directionalLight );
      scene.background = "0x00ffff";
      
      const loader = new THREE.ObjectLoader();
      const geometry = loader.parse(data);
      console.log(geometry);
      const mesh = new THREE.Mesh(geometry);




      /*ressetting the camera*/
      let _boudingBox = new THREE.computeBoundingSphere()
      geometry.computeBoundingBox();
      var bounding_sphere = geometry.boundingSphere;
      console.log(bounding_sphere);
      geometry.computeBoundingBox();
      var bbox = geometry.boundingBox;

      // Compute bounding size:
      let _bounding_size = new THREE.Vector3();

      _bounding_size.subVectors(bbox.max, bbox.min);


      // Cmopute half boundnig size:
      var half_size = new THREE.Vector3(_bounding_size.x, _bounding_size.y,
                                        _bounding_size.z);
      half_size.multiplyScalar(0.5);

      // Compute center. Unfortunately, bounding sphere does not have an
      // up to date center attribute, so compute it from the bounding box.
      var center = new THREE.Vector3();
      center.addVectors(bbox.min, half_size);
      // alert( center.x + ' ' + center.y + ' ' + center.z );
      center.z -= half_size.z;

      var fov_rad = 45 * Math.PI / 180.0;
      const dist = bounding_sphere.radius * radiusScale / Math.sin(fov_rad * 0.5);

      // Translate the mesh so that the center of its projection on the Z=0
      // plane is right ar (0,0) and it is touching the Z=0 plane from bellow.
      var mat = new THREE.Matrix4();
      mat.makeTranslation( -center.x, -center.y,  -center.z );
      mesh.geometry.applyMatrix(mat);

      target = new THREE.Vector3();
      target.set( 0, 0, half_size.z );
      // target.applyMatrix4( mesh.matrixWorld );
      // alert( target.x + ' ' + target.y + ' ' + target.z );

      eye = new THREE.Vector3();
      eye.set(0, -1, 0);
      eye.multiplyScalar(dist);
      eye.add(target);
      controls.target.copy(target);

      up = new THREE.Vector3();
      up.set(0, 0, 1);
      camera.up.copy(up);

      // Move the camera up a bit so that the viewer have a good look at
      // grid, and in particular at the grid units.
      var adjacent = target.y - eye.y;
      var tangent = half_size.z / adjacent;
      const minTangent = 0.578;
      if (tangent < minTangent) {
        eye.z = adjacent * minTangent;
      }
      camera.position.copy(eye);
      camera.lookAt(target);

      // Store the 6 predefined pairs of camera position and orientation:
      let cameras = {};
      var quat = camera.quaternion;
      cameras['home'] = {position: eye.clone(),
                               target: target.clone(),
                               up: up.clone(),
                               quaternion: quat.clone()};

      // Set 6 predefined points of view
      var simple_camera = new THREE.Camera();
      simple_camera.up.copy(up);

      // Front
      eye.set(0, -1, 0);
      eye.multiplyScalar(dist);
      eye.add(target);
      simple_camera.position.copy(eye);
      simple_camera.lookAt(target);
      quat = simple_camera.quaternion;
      cameras['front'] = {position: eye.clone(),
                                target: target.clone(),
                                up: up.clone(),
                                quaternion: quat.clone()};

      // Back
      eye.set(0, 1, 0);
      eye.multiplyScalar(dist);
      eye.add(target);
      simple_camera.position.copy(eye);
      simple_camera.lookAt(target);
      quat = simple_camera.quaternion;
      cameras['back'] = {position: eye.clone(),
                               target: target.clone(),
                               up: up.clone(),
                               quaternion: quat.clone()};

      // Left
      eye.set(-1, 0, 0);
      eye.multiplyScalar(dist);
      eye.add(target);
      simple_camera.position.copy(eye);
      simple_camera.lookAt(target);
      quat = simple_camera.quaternion;
      cameras['left'] = {position: eye.clone(),
                               target: target.clone(),
                               up: up.clone(),
                               quaternion: quat.clone()};

      // Right
      eye.set(1, 0, 0);
      eye.multiplyScalar(dist);
      eye.add(target);
      simple_camera.position.copy(eye);
      simple_camera.lookAt(target);
      quat = simple_camera.quaternion;
      cameras['right'] = {position: eye.clone(),
                                target: target.clone(),
                                up: up.clone(),
                                quaternion: quat.clone()};

      // Bottom & top
      up.set(0, 1, 0);
      simple_camera.up.copy(up);

      // Bottom
      eye.set(0, 0, -1);
      eye.multiplyScalar(dist);
      eye.add(target);
      simple_camera.position.copy(eye);
      simple_camera.lookAt(target);
      quat = simple_camera.quaternion;
      cameras['bottom'] = {position: eye.clone(),
                                 target: target.clone(),
                                 up: up.clone(),
                                 quaternion: quat.clone()};

      // Top
      eye.set(0, 0, 1);
      eye.multiplyScalar(dist);
      eye.add(target);
      simple_camera.position.copy(eye);
      simple_camera.lookAt(target);
      quat = simple_camera.quaternion;
      cameras['top'] = {position: eye.clone(),
                              target: target.clone(),
                              up: up.clone(),
                              quaternion: quat.clone()};

      // var m1 = new THREE.Matrix4();
      // // m1.lookAt( eye, target, up );
      // var zaxis = new THREE.Vector3();
      // zaxis.subVectors( eye, target );
      // zaxis.normalize();
      // var xaxis = new THREE.Vector3();
      // xaxis.crossVectors( up, zaxis );
      // xaxis.normalize();
      // var yaxis = new THREE.Vector3();
      // yaxis.crossVectors( zaxis, xaxis );
      // m1.makeBasis( xaxis, yaxis, zaxis );
      // camera.quaternion.setFromRotationMatrix( m1 );
      const farScale = 0.64;
      camera.far = dist * farScale;
      camera.updateProjectionMatrix();



      /* end of ca;era reset */














      scene.add(geometry);
      
      renderer = new THREE.WebGLRenderer();
      renderer.setPixelRatio( container.devicePixelRatio );
      renderer.setSize( container.offsetWidth, container.offsetHeight );
      container.appendChild( renderer.domElement );

    
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
