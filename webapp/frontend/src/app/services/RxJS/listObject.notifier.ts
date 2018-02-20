import { Injectable } from '@angular/core';
import { BehaviorSubject } from 'rxjs/BehaviorSubject';


@Injectable() 

export class ListObjectNotifier {
    
    private listHasLoaded$: BehaviorSubject<boolean> = new BehaviorSubject<boolean>(false);
    
    constructor() {
    }
    
    getListLoadingStatus = () => {
        return this.listHasLoaded$.asObservable();
    }
    
    setListLoadingStatus = (loaded: boolean) => {
        this.listHasLoaded$.next(loaded);
    }

}