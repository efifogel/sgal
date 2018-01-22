import { Component, OnInit } from '@angular/core';
import { CognitoService } from '../../../services/cognito.service';
import { User } from '../../../models/user';

@Component({
  selector: 'app-forgot',
  templateUrl: './forgot.component.html',
  styleUrls: ['./forgot.component.scss']
})
export class ForgotPasswordComponent {

  user: User = {};

  constructor(private cognito: CognitoService) {
  }

  onForgotSubmit() {
    this.cognito.forgotPassword(this.user);
  }

}
