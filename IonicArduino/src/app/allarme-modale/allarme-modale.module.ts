import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';

import { IonicModule } from '@ionic/angular';

import { AllarmeModalePageRoutingModule } from './allarme-modale-routing.module';

import { AllarmeModalePage } from './allarme-modale.page';

@NgModule({
  imports: [
    CommonModule,
    FormsModule,
    IonicModule,
    AllarmeModalePageRoutingModule
  ],
  declarations: [AllarmeModalePage]
})
export class AllarmeModalePageModule {}
