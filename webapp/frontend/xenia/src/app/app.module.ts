/* Angular Modules */

import {BrowserModule} from '@angular/platform-browser';
import {NgModule} from '@angular/core';
import {RouterModule} from '@angular/router';
import {FormsModule} from '@angular/forms';


/* Base Component */

import {AppComponent} from './app.component';

/* UI Libraries */

import {CovalentLayoutModule} from '@covalent/core/layout';
import {MatInputModule} from '@angular/material/input';

/* Routing & components */

import {AppRoutingModule, RoutableComponents} from './routes.module';

/* Services */

import { CognitoService } from './services/cognito.service';


@NgModule({
  declarations: [
    AppComponent,
    RoutableComponents
  ],
  imports: [
    BrowserModule,
    FormsModule,
    CovalentLayoutModule,
    RouterModule,
    MatInputModule,
    AppRoutingModule
  ],
  providers: [CognitoService],
  bootstrap: [AppComponent]
})
export class AppModule {
}
