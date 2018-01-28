import { environment } from './../../environments/environment';
import { Injectable } from '@angular/core';
import * as AWS from 'aws-sdk/global';
import * as S3 from 'aws-sdk/clients/s3';
import { User } from '../models/user';
import { CognitoCallback } from './cognito.service';

@Injectable()
export class AWS3Service {
  albumBucketName = environment.albumBucketName;
  bucketRegion = environment.bucketRegion;
  IdentityPoolId = environment.identityPoolId;
  s3: any;
  constructor() {
  }
  updateFile = () => {
    AWS.config.update({
      region: this.bucketRegion,
      credentials: new AWS.CognitoIdentityCredentials({
        IdentityPoolId: this.IdentityPoolId
      })
    });
  }
  setBucket = (userId: string) => {
    this.s3 = new S3({
      apiVersion: '2006-03-01',
      params: { Bucket: this.albumBucketName, Prefix: userId }
    });
  }

  listAlbums(userId: string, callback: CognitoCallback) {
    this.updateFile();
    this.setBucket(userId);
    this.s3.listObjects( (error, data) => {
      if (error) {
        callback.cognitoCallback(error, null);
      } else {
        callback.cognitoCallback(null, data);
      }
    });
  }
  getBucketObject(key: string) {
    this.s3.getObject({
      Bucket: this.albumBucketName,
      Key: key
  }, (err, data) => {
      if (err) {
          console.log(err, err.stack);
      } else {
        console.log(data);
      }
  });
  }
}
