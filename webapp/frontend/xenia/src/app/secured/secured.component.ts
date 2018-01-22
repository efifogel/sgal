import { Component, OnInit } from '@angular/core';

@Component({
    selector: 'app-dashboard',
    template: `
    <div class="full-height" id="secured-container">
        <router-outlet></router-outlet>
    </div>`,
    styles: ['#secured-container: {width: 95%; height: 95%}']
})
export class SecuredComponent {

    constructor() { }

}
