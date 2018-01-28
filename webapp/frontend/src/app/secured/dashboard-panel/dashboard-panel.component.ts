import { Component, OnInit, Input } from '@angular/core';

@Component({
  selector: 'app-dashboard-panel',
  templateUrl: './dashboard-panel.component.html',
  styleUrls: ['./dashboard-panel.component.scss']
})
export class DashboardPanelComponent implements OnInit {
  @Input() list: any[];
  constructor() { }

  ngOnInit() {
  }

}
