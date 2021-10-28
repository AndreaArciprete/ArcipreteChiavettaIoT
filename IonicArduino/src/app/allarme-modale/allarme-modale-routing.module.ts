import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { AllarmeModalePage } from './allarme-modale.page';

const routes: Routes = [
  {
    path: '',
    component: AllarmeModalePage
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class AllarmeModalePageRoutingModule {}
