import { Component } from '@angular/core';

declare let AWS: any;
declare let AWSCognito: any;


@Component({
  selector: 'app-home',
  template: `
  <div class="full-height" layout="column" layout-align="center center" id="auth-main">
    <router-outlet></router-outlet>
  </div>`,
  styleUrls: ['../../app.component.css']
})
export class AuthentificationComponent {

  constructor() {
  }

}
