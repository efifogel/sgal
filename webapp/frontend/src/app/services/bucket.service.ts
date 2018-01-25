import { environment } from './../../environments/environment';
import { Injectable } from '@angular/core';
import * as AWS from 'aws-sdk/global';
import * as S3 from 'aws-sdk/clients/s3';
import { User } from '../models/user';

@Injectable()
export class AWSBucketService {
  constructor() {}
  private getS3(): any {
    AWS.config.update({
      region: environment.bucketRegion
    });

    const clientParams: any = {
      region: environment.bucketRegion,
      apiVersion: '2006-03-01',
      params: { Bucket: environment.rekognitionBucket }
    };
    if (environment.s3_endpoint) {
      clientParams.endpoint = environment.s3_endpoint;
    }
    const s3 = new S3(clientParams);

    return s3;
  }
}
