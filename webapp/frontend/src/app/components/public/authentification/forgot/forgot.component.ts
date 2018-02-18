import { Component, OnInit } from '@angular/core';
import { CognitoService } from '../../../../services/AWS/cognito.service';
import { User } from '../../../../models/user';

@Component({
  selector: 'app-forgot',
  templateUrl: './forgot.component.html',
  styleUrls: ['./forgot.component.scss']
})
export class ForgotPasswordComponent {

  user: User = {};
  errorMessage: string;

  constructor(private cognito: CognitoService) {
  }

  onForgotSubmit() {
    this.cognito.forgotPassword(this.user, this);
  }

  cognitoCallback(errMessage: string, result: any) {
    if (errMessage != null) {
      this.errorMessage = errMessage;
    } else {
      console.log(result);
    }
  }

}
