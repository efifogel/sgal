import { Component, AfterViewInit, ViewChild, ElementRef } from '@angular/core';
import { HTTPStatus } from './services/RxJS/HTTPListener.service';
declare var bodymovin: any;

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements AfterViewInit {
  @ViewChild('HTTPLoader') HTTPLoader: ElementRef;
  HTTPActivity: boolean;
  constructor(private httpStatus: HTTPStatus) {
    this.httpStatus.getHttpStatus().subscribe((status: boolean) => this.HTTPActivity = status );
  }
  ngAfterViewInit() {
    const animData = {
      wrapper: this.HTTPLoader.nativeElement,
      animType: 'svg',
      loop: true,
      prerender: true,
      autoplay: true,
      path: 'https://s3-us-west-2.amazonaws.com/s.cdpn.io/35984/LEGO_loader.json'
    };
    const anim = bodymovin.loadAnimation(animData);
  anim.setSpeed(3.4);
  }
}
