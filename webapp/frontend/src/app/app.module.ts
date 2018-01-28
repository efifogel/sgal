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
import { AWS3Service } from './services/bucket.service';

/* Utils (Pipes ..) */

import { TrimStringPipe } from './utils/pipes/trimString';

@NgModule({
  declarations: [
    AppComponent,
    RoutableComponents,
    TrimStringPipe
  ],
  imports: [
    BrowserModule,
    FormsModule,
    CovalentLayoutModule,
    RouterModule,
    MatInputModule,
    AppRoutingModule
  ],
  providers: [CognitoService, AWS3Service],
  bootstrap: [AppComponent]
})
export class AppModule {
}
