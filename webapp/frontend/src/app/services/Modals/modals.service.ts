import { Injectable } from '@angular/core';
import * as modals from './index'
import {NgbModal, ModalDismissReasons} from '@ng-bootstrap/ng-bootstrap';


@Injectable()
export class ModalService {
  allModals = modals.allModals;
  constructor(private modalService: NgbModal) { }

  showErrorModal = (errorMessage: string) => {
    console.log(errorMessage);
    const modalRef = this.modalService.open(this.allModals.error, { windowClass: 'dark-modal' });
    modalRef.componentInstance.message = errorMessage;
  }
}
