import { Component } from '@angular/core';
import { Router } from '@angular/router';
import { User } from '../../../../models/user';
import { CognitoService } from '../../../../services/AWS/cognito.service';

@Component({
  selector: 'app-register',
  templateUrl: './register.component.html',
  styleUrls: ['./register.component.css']
})
export class RegisterComponent {

  user: User = {};
  errorMessage: string;

  constructor(private router: Router, private cognito: CognitoService) {
  }

  registerUser = (user: User) => {
    this.errorMessage = null;
    this.cognito.register(user, this);
  }
  cognitoCallback(message: string, result: any) {
    if (message != null) {
      this.errorMessage = message;
    } else {
      this.router.navigate(['/auth/confirmAccount', result.user.username]);
    }
  }
}
