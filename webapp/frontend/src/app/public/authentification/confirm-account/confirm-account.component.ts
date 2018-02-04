import { Component, OnInit, OnDestroy } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { CognitoService } from '../../../services/AWS/cognito.service';

@Component({
  selector: 'app-confirm-account',
  templateUrl: './confirm-account.component.html',
  styleUrls: ['./confirm-account.component.scss']
})
export class ConfirmAccountComponent implements OnInit, OnDestroy {
  private sub: any;
  email: string;
  errorMessage: string;
  code: string;

  constructor(public route: ActivatedRoute, private cognito: CognitoService, private router: Router) { }

  ngOnInit() {
    this.sub = this.route.params.subscribe(params => {
      this.email = params['username'];
    });
    this.errorMessage = null;
  }

  ngOnDestroy() {
    this.sub.unsubscribe();
  }

  confirmCode = () => {
    this.cognito.verifyConfirmationCode(this.email, this.code, this);
  }

  cognitoCallback(errMessage: string, result: any) {
    if (errMessage != null) {
      this.errorMessage = errMessage;
    } else {
      this.router.navigate(['/secured']);
    }
  }

}
