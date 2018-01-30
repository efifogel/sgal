import {
  Pipe,
  PipeTransform,
  Directive,
  ElementRef,
  AfterViewInit,
  Input
} from '@angular/core';

@Pipe({ name: 'trimString' })
export class TrimStringPipe implements PipeTransform {
  transform(str: string): string {
    return str.substring(str.indexOf('/') + 1);
  }
}
