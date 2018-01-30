import { S3Callback } from './bucket.service';
import { environment } from './../../environments/environment';
import { Injectable } from '@angular/core';
import * as AWS from 'aws-sdk/global';
import * as S3 from 'aws-sdk/clients/s3';
import { User } from '../models/user';
import { CognitoCallback } from './cognito.service';

export interface S3Callback {
  ListobjectCallBack(message: string, result: any): void;
  UploadCallback(message: string, result: any): void;
  GetObjectCallback(message: string, result: any): void;
}

@Injectable()
export class AWS3Service {
  albumBucketName = environment.albumBucketName;
  bucketRegion = environment.bucketRegion;
  IdentityPoolId = environment.identityPoolId;
  s3: any;
  constructor() {
    this.setBucketConfig();
    const userId = JSON.parse(window.localStorage.getItem('cognitoClientId'));
    this.setBucketParams(userId);
  }
  setBucketConfig = () => {
    AWS.config.update({
      region: this.bucketRegion,
      credentials: new AWS.CognitoIdentityCredentials({
        IdentityPoolId: this.IdentityPoolId
      })
    });
  }
  setBucketParams = (userId: string) => {
    this.s3 = new S3({
      apiVersion: '2006-03-01',
      params: { Bucket: this.albumBucketName, Prefix: userId }
    });
  }

  listAlbums(userId: string, callback: S3Callback) {
    this.s3.listObjects((error, data) => {
      if (error) {
        callback.ListobjectCallBack(error, null);
      } else {
        callback.ListobjectCallBack(null, data);
      }
    });
  }
  getBucketObject(key: string, callback: S3Callback) {
    this.s3.getObject({
      Bucket: this.albumBucketName,
      Key: key
  }, (err, data) => {
      if (err) {
        callback.GetObjectCallback(err.message, null);
      } else {
        callback.GetObjectCallback(null, data);
      }
  });
  }

  uploadDocumentToBucket(dir: string, files: any, callback: S3Callback) {
    if (!files.length) {
      callback.UploadCallback('Please chose a file to upload', null);
    }
    const file = files[0];
    const fileName = file.name;
    const fileNameKey = encodeURIComponent(dir) + '/';
    const fileKey = fileNameKey + fileName;
    const params = {
      Bucket: this.albumBucketName,
      Key: fileKey,
      Body: file,
      ACL: 'public-read-write'
    };
    console.log(params);
    this.s3.upload(params, (err, data) => {
      if (err) {
        callback.UploadCallback(err.message, null);
      } else {
        callback.UploadCallback(null, data);
      }
    });
  }
}
