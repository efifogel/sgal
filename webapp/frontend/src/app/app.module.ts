/* Angular Modules */

import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { RouterModule } from '@angular/router';
import { FormsModule } from '@angular/forms';
import { HttpClientModule, HTTP_INTERCEPTORS } from '@angular/common/http';

/* Base Component */

import { AppComponent } from './app.component';

/* UI Libraries */

import { CovalentLayoutModule } from '@covalent/core/layout';
import {NoopAnimationsModule} from '@angular/platform-browser/animations';
import {MatSnackBarModule} from '@angular/material/snack-bar';

/* Routing & components */

import { AppRoutingModule, RoutableComponents } from './routes.module';

/* Services */
// AWS
import { CognitoService } from './services/AWS/cognito.service';
import { AWS3Service } from './services/AWS/bucket.service';

const AWS_Services = [CognitoService, AWS3Service];

// HANDLERS
import { ModalService } from './services/Modals/modals.service';

const ModalServices = [ModalService];

// Intercept all HTTP request and throws an event that will show the loader as soon as any HTTP activity is detected (GET || POST || PUT || DELETE)
// explanations here  => https://medium.com/@johnmeguira/intercept-all-http-calls-with-angular-5-to-display-a-loader-281924b73ad8

import { HTTPListener, HTTPStatus } from './services/RxJS/HTTPListener.service';
import { PlayerNotifier } from './services/RxJS/player.notify.service';

const RxJS_Services = [HTTPListener, HTTPStatus, PlayerNotifier];

/* Utils (Pipes ..) */

import { TrimStringPipe } from './utils/pipes/trimString';

const Pipes = [TrimStringPipe];

/* route guards, preventing a user from accessing secured components if he is not loggedin */

import { AuthGuard } from './services/guards/routes.guard';

const Guards = [AuthGuard];

// modals
import { NgbModule, NgbActiveModal } from '@ng-bootstrap/ng-bootstrap';
import { ErrorModalComponent } from './shared/modals/error-modal/error-modal.component';

const Modals = [ErrorModalComponent];

@NgModule({
  declarations: [AppComponent, RoutableComponents, TrimStringPipe, Modals],
  imports: [
    BrowserModule,
    NoopAnimationsModule,
    FormsModule,
    CovalentLayoutModule,
    RouterModule,
    MatSnackBarModule,
    NgbModule.forRoot(),
    AppRoutingModule
  ],
  providers: [
    ...AWS_Services,
    ...RxJS_Services,
    ...Pipes,
    ...ModalServices,
    ...Guards,
    {
      provide: HTTP_INTERCEPTORS,
      useClass: HTTPListener,
      multi: true
    }
  ],
  entryComponents: [...Modals],
  bootstrap: [AppComponent]
})
export class AppModule {}
