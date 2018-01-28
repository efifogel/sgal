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
  documentTitle: string;
  files: any[];
  list = ['File1.wrl', 'file2.wrl', 'file3.wrl', 'file4.wrl', 'file5.wrl'];
  clientId: string;
  constructor(private s3: AWS3Service) {
    this.clientId = localStorage.getItem('cognitoClientId');
    console.log(this.clientId);
    this.s3.listAlbums('2lar6nm448283r9khoegrd7p8h', this);
  }
  onFileUpload(event: any) {
    const element = event.element;
    this.documentTitle = element.files[0].name;
    this.wrlFile = element.target.value;
  }
  broadcastFileName(title: string) {
    this.documentTitle = title;
  }

  cognitoCallback(errorMessage: string, data: any) {
    if (!errorMessage) {
      this.files = data.Contents.splice(0, 1);
      this.s3.getBucketObject(data.Contents[0].Key);
      console.log(data);
    } else {
      alert(errorMessage);
    }
  }
}
