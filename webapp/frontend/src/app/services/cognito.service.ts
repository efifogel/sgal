import { environment } from './../../environments/environment';
import { Injectable } from '@angular/core';
import * as AWSCognito from 'amazon-cognito-identity-js';
import { User } from '../models/user';


interface CognitoCallback {
  cognitoCallback(message: string, result: any): void;
}


@Injectable()
export class CognitoService {
  public static _REGION = environment.region;
  public static _IDENTITY_POOL_ID = environment.identityPoolId;
  public static _USER_POOL_ID = environment.userPoolId;
  public static _CLIENT_ID = environment.clientId;

  poolData = { UserPoolId: CognitoService._USER_POOL_ID, ClientId: CognitoService._CLIENT_ID };
  userPool = new AWSCognito.CognitoUserPool(this.poolData);

  constructor() {
  }
  getUserPool() {
    return this.userPool;
  }
  register(user: User, callback: CognitoCallback) {
    const attributeList = [];
    const dataNickname = { Name: 'nickname', Value: user.name };
    const dataEmail = { Name: 'email', Value: user.email };
    attributeList.push(new AWSCognito.CognitoUserAttribute(dataEmail));
    attributeList.push(new AWSCognito.CognitoUserAttribute(dataNickname));
    this.userPool.signUp(user.email, user.password, attributeList, null, (err, res) => {
      if (err) {
        callback.cognitoCallback(err.message, null);
      } else {
        callback.cognitoCallback(null, res);
      }
    });
  }

  verifyConfirmationCode(username: string, confirmationCode: string, callback: CognitoCallback) {
    const userData = { Username: username, Pool: this.userPool };
    const cognitoUser = new AWSCognito.CognitoUser(userData);
    cognitoUser.confirmRegistration(confirmationCode, true, (err, result) => {
      if (err) {
        callback.cognitoCallback(err.message, null);
        return;
      }
      callback.cognitoCallback(null, result);
    });
  }

  authenticateUser(user: User, callback: CognitoCallback) {
    const authenticationData = {
      Username: user.email,
      Password: user.password,
    };
    const userData = {
      Username: user.email,
      Pool: this.userPool
    };
    const authenticationDetails = new AWSCognito.AuthenticationDetails(authenticationData);
    const cognitoUser = new AWSCognito.CognitoUser(userData);
    cognitoUser.authenticateUser(authenticationDetails, {
      onSuccess: (result) => {
        callback.cognitoCallback(null, result);
      },
      onFailure: (err) => {
        callback.cognitoCallback(err.message, null);
      },

    });
  }
  forgotPassword(user: User, callback: CognitoCallback) {
    const userData = {
      Username: user.email,
      Pool: this.userPool
    };
    const cognitoUser = new AWSCognito.CognitoUser(userData);
    cognitoUser.forgotPassword({
      onSuccess: function (result) {
        callback.cognitoCallback(null, result);
      },
      onFailure: function (err) {
        callback.cognitoCallback(err.message, null);
      },
      inputVerificationCode() {
        const verificationCode = prompt('Please enter the verification code ', '');
        const newPassword = prompt('Enter new password ', '');
        cognitoUser.confirmPassword(verificationCode, newPassword, {
          onSuccess: () => {
            callback.cognitoCallback(null, true);
          },
          onFailure: (error) => {
            callback.cognitoCallback(error.message, false);
          }
        });
      }
    });
  }

}
