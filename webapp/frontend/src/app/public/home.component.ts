import { Component } from '@angular/core';

declare let AWS: any;
declare let AWSCognito: any;


@Component({
  selector: 'app-home',
  template: `
  <div class="full-height" layout="column" layout-align="center center">
    <router-outlet></router-outlet>
  </div>`
})
export class HomeComponent {

  constructor() {
  }

}
