import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';

@Component({
  selector: 'app-dashboard-panel',
  templateUrl: './dashboard-panel.component.html',
  styleUrls: ['./dashboard-panel.component.scss']
})
export class DashboardPanelComponent implements OnInit {
  @Input()
  files: any[];
  dataHasLoaded: boolean;
  // tslint:disable-next-line:no-output-on-prefix
  @Output()
  onFileUpload: EventEmitter<any> = new EventEmitter<any>();
  // tslint:disable-next-line:no-output-on-prefix
  @Output()
  onFileClick: EventEmitter<string> = new EventEmitter<string>();
  constructor() { }
  ngOnInit() {
  }
  onFileSelected(event: any) {
    this.onFileUpload.emit({ element: event });
  }
  selectElement(key: string) {
    this.onFileClick.emit(key);
  }
}
