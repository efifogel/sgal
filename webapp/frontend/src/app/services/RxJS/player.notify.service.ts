import { Injectable } from '@angular/core';
import {BehaviorSubject} from 'rxjs/BehaviorSubject';

@Injectable()

export class PlayerNotifier {
    private selectedFile$: BehaviorSubject<any>;
    constructor() {
        this.selectedFile$ = new BehaviorSubject<any>('');
    }
    setSelectedFile(buffer: any) {
        this.selectedFile$.next(buffer);
    }
    getSelectedFile = () => {
        return this.selectedFile$.asObservable();
    }
}