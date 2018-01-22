import { Component } from '@angular/core';
import { Router } from '@angular/router';
import { User } from '../../../models/user';
import { CognitoService } from '../../../services/cognito.service';

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.scss']
})
export class LoginComponent {

  user: User = {};
  errorMessage: string;
  token: string;

  constructor(private cognito: CognitoService, private router: Router) { }

  login = (user: User) => {
    this.cognito.authenticateUser(this.user, this);
  }

  cognitoCallback(errorMessage: string, result: any) {
    if (errorMessage != null) {
      this.errorMessage = errorMessage;
    } else {
      this.token = result.getAccessToken().getJwtToken();
      this.router.navigate(['/secured/dashboard']);
    }
  }
}
