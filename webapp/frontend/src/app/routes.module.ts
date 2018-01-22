import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { HomeComponent } from './public/home.component';
import { RegisterComponent } from './public/auth/register/register.component';
import { LoginComponent } from './public/auth/login/login.component';
import { ConfirmAccountComponent } from './public/auth/confirm-account/confirm-account.component';
import { DashboardComponent } from './secured/dashboard/dashboard.component';
import { SecuredComponent } from './secured/secured.component';


const homeRoutes: Routes = [
  { path: '', redirectTo: 'home', pathMatch: 'full' },
  {
    path: 'home', component: HomeComponent,
    children: [
      { path: 'register', component: RegisterComponent },
      { path: 'confirmAccount/:username', component: ConfirmAccountComponent },
      { path: 'login', component: LoginComponent }
    ]
  }
];

const secureHomeRoutes: Routes = [
  {

    path: '',
    redirectTo: '/secured',
    pathMatch: 'full'
  },
  {
    path: 'secured', component: SecuredComponent,
    children: [
      { path: 'dashboard', component: DashboardComponent }
    ]
  }
];


const routes: Routes = [
  {
    path: '',
    children: [
      ...homeRoutes,
      ...secureHomeRoutes,
      {
        path: '',
        component: HomeComponent
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
  HomeComponent,
  RegisterComponent,
  LoginComponent,
  ConfirmAccountComponent,
  SecuredComponent,
  DashboardComponent
];
