import {Component, Input, OnInit} from '@angular/core';
import {Topic} from '../interfaces/topic';
import {ControlloRemotoService} from '../services/controllo-remoto.service';
import {ModalController} from '@ionic/angular';

@Component({
  selector: 'app-allarme-modale',
  templateUrl: './allarme-modale.page.html',
  styleUrls: ['./allarme-modale.page.scss'],
})
export class AllarmeModalePage implements OnInit {

  @Input() topic: Topic;
  constructor(
      private controlloRemotoService: ControlloRemotoService,
      private modalCtrl: ModalController
  ) { }

  ngOnInit() {
  }

  spegniAllarme(){
    this.controlloRemotoService.spegniAllarme(this.topic.nomeAllarme);
    this.dismiss();
  }

  async dismiss() {

    await this.modalCtrl.dismiss();

  }


}
