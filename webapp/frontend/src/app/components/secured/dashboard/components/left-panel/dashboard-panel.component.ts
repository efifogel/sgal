import { Component, Input, Output, EventEmitter } from '@angular/core';
import { ListObjectNotifier } from '../../../../../services/RxJS/listObject.notifier';

@Component({
  selector: 'app-dashboard-panel',
  templateUrl: './dashboard-panel.component.html',
  styleUrls: ['./dashboard-panel.component.scss']
})
export class DashboardPanelComponent {
  @Input()
  files: any[];
  selectedElement: string;
  @Output() onFileUpload: EventEmitter<any> = new EventEmitter<any>();
  @Output() onFileClick: EventEmitter<string> = new EventEmitter<string>();
  constructor(private listObjectNotifier: ListObjectNotifier) {
    this.listObjectNotifier.getListLoadingStatus()
      .subscribe(loaded => loaded ? this.selectedElement = this.files[0].Key : '');
  }
  
  onFileSelected(event: any) {
    this.onFileUpload.emit({ element: event });
  }
  selectElement(key: string) {
    this.onFileClick.emit(key);
    this.selectedElement = key;
  }
  isSelected(key: string) {
    return this.selectedElement === key;
  }
}
