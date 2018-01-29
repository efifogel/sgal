import { Component } from '@angular/core';
import { AWS3Service } from '../../services/bucket.service';
import * as S3 from 'aws-sdk/clients/s3';

@Component({
  selector: 'app-dashboard',
  templateUrl: './dashboard.component.html',
  styleUrls: ['./dashboard.component.scss']
})
export class DashboardComponent {
  dataHasLoaded = false;
  wrlFile: any;
  fileBody: any;
  documentTitle: string;
  files: any[];
  clientId: string;
  constructor(private s3: AWS3Service) {
    this.clientId = JSON.parse(localStorage.getItem('cognitoClientId'));
    this.s3.listAlbums(this.clientId, this);
  }
  getFileObject(key: string) {
    this.s3.getBucketObject(key, this);
  }

  uploadFile(event: any) {
    const files = event.element.target.files;
    const directory = JSON.parse(localStorage.getItem('cognitoClientId'));
    this.s3.uploadDocumentToBucket(directory, files, this);
  }
  /* Callbacks interfaces */
  ListobjectCallBack(errorMessage: string, data: any) {
    if (!errorMessage) {
      if (data.Contents.length) {
        data.Contents.shift();
        this.s3.getBucketObject(data.Contents[0].Key, this);
        this.files = data.Contents;
      }
    } else {
      alert(errorMessage);
    }
  }
  UploadCallback(errorMessage: string, data: any) {
    if (!errorMessage) {
      this.s3.listAlbums(this.clientId, this);
    } else {
      alert(errorMessage);
    }
  }
  GetObjectCallback(errorMessage: string, data: any) {
    if (!errorMessage) {
      console.log(data);
      this.wrlFile = data.Body;
    } else {
      alert(errorMessage);
    }
  }

}
