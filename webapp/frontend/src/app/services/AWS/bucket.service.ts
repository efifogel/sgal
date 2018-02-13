import { HTTPStatus } from "./../RxJS/HTTPListener.service";
import { S3Callback } from "./bucket.service";
import { environment } from "./../../../environments/environment";
import { Injectable } from "@angular/core";
import * as AWS from "aws-sdk/global";
import * as S3 from "aws-sdk/clients/s3";
import { User } from "../../models/user";
import { CognitoCallback } from "./cognito.service";
import { ModalService } from '../Modals/modals.service';

export interface S3Callback {
  ListobjectCallBack(message: string, result: any): void;
  UploadCallback(message: string, result: any): void;
  GetObjectCallback(filetype: string, message: string, result: any): void;
}

@Injectable()
export class AWS3Service {
  albumBucketName = environment.albumBucketName;
  bucketRegion = environment.bucketRegion;
  IdentityPoolId = environment.identityPoolId;
  s3: any;
  constructor(private httpStatus: HTTPStatus, private modalService: ModalService) {
    this.setBucketConfig();
    const userId = JSON.parse(window.localStorage.getItem("cognitoClientId"));
    this.setBucketParams(userId);
  }
  setBucketConfig = () => {
    AWS.config.update({
      region: this.bucketRegion,
      credentials: new AWS.CognitoIdentityCredentials({
        IdentityPoolId: this.IdentityPoolId
      })
    });
  };
  setBucketParams = (userId: string) => {
    this.s3 = new S3({
      apiVersion: "2006-03-01",
      params: { Bucket: this.albumBucketName, Prefix: userId }
    });
  };

  listAlbums(userId: string, callback: S3Callback) {
    this.httpStatus.setHttpStatus(true);
    this.s3.listObjects((error, data) => {
      if (error) {
        this.modalService.showErrorModal(error.message);
        callback.ListobjectCallBack(error, null);
      } else {
        console.log(data);
        this.modalService.showErrorModal('Oups, some error occured, this is all we know');
        callback.ListobjectCallBack(null, data);
      }
      this.httpStatus.setHttpStatus(false);
    });
  }
  getBucketObject(filetype: string, key: string,  callback: S3Callback) {
    this.httpStatus.setHttpStatus(true);
    this.s3.getObject(
      {
        Bucket: this.albumBucketName,
        Key: key
      },
      (err, data) => {
        if (err) {
          this.modalService.showErrorModal(err.message);
          callback.GetObjectCallback(filetype, err.message, null);
        } else {
          callback.GetObjectCallback(filetype, null, data);
        }
        this.httpStatus.setHttpStatus(false);
      }
    );
  }
  uploadDocumentToBucket(dir: string, files: any, callback: S3Callback) {
    this.httpStatus.setHttpStatus(true);
    if (!files.length) {
      callback.UploadCallback("Please chose a file to upload", null);
    }
    const file = files[0];
    file.extension = "wrl";
    const fileName = file.name;
    const fileNameKey = encodeURIComponent(dir) + "/";
    const fileKey = fileNameKey + fileName;
    const params = {
      Bucket: this.albumBucketName,
      Key: fileKey,
      Body: file,
      ACL: "public-read-write"
    };
    this.s3.upload(params, (err, data) => {
      if (err) {
        this.modalService.showErrorModal(err.message);
        callback.UploadCallback(err.message, null);
      } else {
        callback.UploadCallback(null, data);
      }
      this.httpStatus.setHttpStatus(false);
    });
  }
}
