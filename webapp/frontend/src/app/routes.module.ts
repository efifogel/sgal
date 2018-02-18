import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { AuthentificationComponent } from './components/public/authentification.component';
import { RegisterComponent } from './components/public/authentification/register/register.component';
import { LoginComponent } from './components/public/authentification/login/login.component';
import { ConfirmAccountComponent } from './components/public/authentification/confirm-account/confirm-account.component';
import { DashboardComponent } from './components/secured/dashboard/dashboard.component';
import { SecuredComponent } from './components/secured/secured.component';
import { ForgotPasswordComponent } from './components/public/authentification/forgot/forgot.component';
import { SceneComponent } from './components/secured/dashboard/components/scene/scene.component';
import { DashboardPanelComponent } from './components/secured/dashboard/components/left-panel/dashboard-panel.component';
import { AuthGuard } from './services/guards/routes.guard';

const authRoutes: Routes = [
  { path: '', redirectTo: 'auth/login', pathMatch: 'full' },
  {
    path: 'auth', component: AuthentificationComponent,
    children: [
      { path: 'register', component: RegisterComponent },
      { path: 'confirmAccount/:username', component: ConfirmAccountComponent },
      { path: 'login', component: LoginComponent },
      { path: 'forgotPassword', component: ForgotPasswordComponent }
    ]
  }
];

const secureHomeRoutes: Routes = [
  {
    path: '',
    redirectTo: '/secured/dashboard',
    pathMatch: 'full'
  },
  {
    path: 'secured', component: SecuredComponent, canActivate: [AuthGuard],

    children: [
      { path: 'dashboard', component: DashboardComponent },
      { path: 'scene', component: SceneComponent }
    ]
  }
];


const routes: Routes = [
  {
    path: '',
    children: [
      ...authRoutes,
      ...secureHomeRoutes,
      {
        path: '',
        component: LoginComponent
      }
    ]
  },
];

@NgModule({
  imports: [RouterModule.forRoot(routes, { useHash: true })],
  exports: [RouterModule]
})

export class AppRoutingModule { }


export const RoutableComponents = [
  AuthentificationComponent,
  RegisterComponent,
  LoginComponent,
  ForgotPasswordComponent,
  ConfirmAccountComponent,
  SecuredComponent,
  DashboardComponent,
  SceneComponent,
  DashboardPanelComponent
];
