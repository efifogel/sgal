import { Component, AfterViewInit, ViewChild, ElementRef } from '@angular/core';
import { HTTPStatus } from './services/RxJS/HTTPListener.service';
import { Router, NavigationEnd } from '@angular/router';
import { CognitoService } from './services/AWS/cognito.service';

declare var bodymovin: any;
declare var $: any;

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements AfterViewInit {
  @ViewChild('HTTPLoader') HTTPLoader: ElementRef;
  isSecuredRoute: boolean;
  currentUser: string;
  HTTPActivity: boolean;
  constructor(private httpStatus: HTTPStatus, private router: Router, private cognito: CognitoService) {
    this.httpStatus.getHttpStatus().subscribe((status: boolean) => this.HTTPActivity = status );
    this.router.events.subscribe(event => {
      const navigationEnded: boolean = event instanceof NavigationEnd;
      if (navigationEnded) {
        this.isSecuredRoute = (this.router.url.includes('secured') ? true : false);
        this.currentUser = localStorage.getItem('username');
      }
    })
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

  logout() {
    this.cognito.logout();
    localStorage.clear();
    this.router.navigate(['auth/login']);
  }
}
