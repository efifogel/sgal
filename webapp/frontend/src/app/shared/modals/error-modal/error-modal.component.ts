import { Component, OnInit, Input, ViewEncapsulation } from '@angular/core';
import { NgbActiveModal } from '@ng-bootstrap/ng-bootstrap';

@Component({
  selector: 'app-error-modal',
  templateUrl: './error-modal.component.html',
  styleUrls: ['./error-modal.component.scss'],
  encapsulation: ViewEncapsulation.None,
  styles: [`
    .dark-modal .modal-header {
      background-color: #d9534f;
      color: white;
    }
    .dark-modal .modal-content {
      background-color: #292D3E;
      color: white;
    }
    .dark-modal .modal-footer {
      border: solid 0px;
    }
  `]
})
export class ErrorModalComponent {
@Input()
message: string;

  constructor(public activeModal: NgbActiveModal) { }

}
