import { Injectable } from '@angular/core';
import {
    Router,
    CanActivate,
    ActivatedRouteSnapshot,
    RouterStateSnapshot
} from '@angular/router';


@Injectable()
export class AuthGuard implements CanActivate {

    constructor(private router: Router) {
    }

    canActivate(route: ActivatedRouteSnapshot, state: RouterStateSnapshot) {
        const { url } = state;
        const cognitoLS = localStorage.getItem('cognitoClientId');
        if (!cognitoLS) this.router.navigate(['/auth/login']);
        return cognitoLS.length > 5;
    }
}