import {Component} from "@angular/core";
import {AWS3Service} from "../../../services/AWS/bucket.service";
import * as S3 from "aws-sdk/clients/s3";
import {PlayerNotifier} from "../../../services/RxJS/player.notify.service";
import { ModalService } from "../../../services/Modals/modals.service";
import {MatSnackBar} from '@angular/material';

declare var TextDecoder: any;
declare var $: any;

@Component({
  selector: "app-dashboard",
  templateUrl: "./dashboard.component.html",
  styleUrls: ["./dashboard.component.scss"]
})
export class DashboardComponent {
  dataHasLoaded = false;
  wrlFile: any;
  fileBody: any;
  documentTitle: string;
  files: any[];
  clientId: string;
  
  constructor(private s3: AWS3Service, private playerNotifier: PlayerNotifier,
    private modalService: ModalService, public snackBar: MatSnackBar) {
      this.clientId = JSON.parse(localStorage.getItem("cognitoClientId"));
      this.s3.listAlbums(this.clientId, this);
    }
    
    getFileObject(key: string) {
      this.showSnackbar("Parsing document ...");
      if (key.includes('.wrl')) {
        this.s3.getBucketObject('wrl', key, this);
      } else if (key.includes('dxf')) {
        this.s3.getBucketObject('dxf', key, this);
      } else if (key.includes('json')) {
        this.s3.getBucketObject('json', key, this);
      }
    }
    
    uploadFile(event: any) {
      const files = event.element.target.files;
      const directory = JSON.parse(localStorage.getItem("cognitoClientId"));
      this.s3.uploadDocumentToBucket(directory, files, this);
    }
    
    /* Callbacks interfaces */
    ListobjectCallBack(errorMessage: string, data: any) {
      if (!errorMessage) {
        if (data.Contents.length > 1) {
          data.Contents.shift();
          this.s3.getBucketObject('', data.Contents[0].Key, this);
          this.files = data.Contents;
        }
      } else {
        this.modalService.showErrorModal("Failed to fetch the list of document(s)");
      }
    }
    
    UploadCallback(errorMessage: string, data: any) {
      if (!errorMessage) {
        const newKey = data.Key;
        this.snackBar.open("Opening the document ...");
        this.s3.listAlbums(this.clientId, this);
        this.getFileObject(newKey);
      } else {
        this.modalService.showErrorModal("Failed to upload this document");
      }
    }
    
    GetObjectCallback(filetype: string, errorMessage: string, data: any) {
      if (!errorMessage) {
        if (filetype === 'json') {
          const jsonData =  this.binArrayToJson(data.Body);
          const fileToSend = {type: filetype, buffer: jsonData};
          this.playerNotifier.setSelectedFile(fileToSend);
        } else {
          const fileToWrite = new TextDecoder("utf-8").decode(data.Body);
          this.wrlFile = fileToWrite;
          const file = {type: filetype, buffer: fileToWrite};
          this.playerNotifier.setSelectedFile(file);
        }
      } else {
        this.modalService.showErrorModal("Failed to download this document");
      }
    }
    
    binArrayToJson = (binArray) => {
      let str = "";
      for (let i = 0; i < binArray.length; i++) {
        str += String.fromCharCode(parseInt(binArray[i]));
      }
      return JSON.parse(str);
    }
    showSnackbar(message: string) {
      const snackRef = this.snackBar.open(message);
      setTimeout(() => {
        snackRef.dismiss();
      }, 1000);
    }
  }
  