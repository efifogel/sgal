import { Component } from '@angular/core';

@Component({
  selector: 'app-dashboard',
  templateUrl: './dashboard.component.html',
  styleUrls: ['./dashboard.component.scss']
})
export class DashboardComponent {
  dataHasLoaded = false;
  wrlFile: any;
  documentTitle: string;
  list = ['File1.wrl', 'file2.wrl', 'file3.wrl', 'file4.wrl', 'file5.wrl'];
  constructor() { }
  onFileUpload(event: any) {
    const element = event.element;
    this.documentTitle = element.files[0].name;
    this.wrlFile = element.target.value;
  }
  broadcastFileName(title: string) {
    this.documentTitle = title;
  }
}
