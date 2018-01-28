import {Pipe, PipeTransform, Directive, ElementRef, AfterViewInit, Input} from '@angular/core';


@Pipe({name: 'trimStringUntil'})
export class TrimStringPipe implements PipeTransform {
  transform(text: string, delimiter: string): string {
   return text + delimiter;
  }
}