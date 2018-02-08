export interface DXFJSON {
  headers: any;
  blocks: any;
  tables: any;
  entities: Entity[];
}

export interface Entity {
  handle: string;
  layer: string;
  ownerHandle: string;
  type: string;
  vertices?: Vertice[];
}

export interface Vertice {
  x: number;
  y: number;
  z?: number;
  bulge?: number;
}
