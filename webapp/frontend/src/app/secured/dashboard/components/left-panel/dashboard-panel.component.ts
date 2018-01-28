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
  onFileSelection: EventEmitter<string> = new EventEmitter<string>();
  constructor() { }

  ngOnInit() {
  }
  selectElement(title: string) {
    this.onFileSelection.emit(title);
  }
  onFileSelected(event: any) {
    this.onFileUpload.emit({ element: event.target });
  }
}
