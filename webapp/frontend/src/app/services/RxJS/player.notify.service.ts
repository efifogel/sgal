import { Injectable } from '@angular/core';
import {BehaviorSubject} from 'rxjs/BehaviorSubject';

interface SelectedBuffer {
    type?: string;
    buffer?: any;
}

@Injectable()

export class PlayerNotifier {
    private selectedFile$: BehaviorSubject<SelectedBuffer>;
    constructor() {
        this.selectedFile$ = new BehaviorSubject<SelectedBuffer>({});
    }
    setSelectedFile(file: any) {
        this.selectedFile$.next(file);
    }
    getSelectedFile = () => {
        return this.selectedFile$.asObservable();
    }
}