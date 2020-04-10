#include "v_rootlpdiag.h"

#include "v_lpdiag.h"
#include <QLineF>
#include "qmath.h"
#include "qpfunction.h"



v_RootLPDiag::v_RootLPDiag(QObject *parent):
    VisualElements(parent)
{
    setObjectName("Группа элеметов 'отображение диагностики ЛП'");

//    FDEFAULTLPSIZE=QSizeF(80,120);
//    FDEFAULTLPNAMESIZE=FDEFAULTLPSIZE.height()/8;
}





void v_RootLPDiag::updateAfterLoad()
{
    VisualElements::updateAfterLoad();
}


QPointF pointInList(QPointF P1,QList<QPointF> *lp)
{
    QPointF PG1=lp->at(0);
    for (int i=1;i<lp->size();i++){
        if ( (qFabs(PG1.x()-lp->at(i).x())+qFabs(PG1.y()-lp->at(i).y())) >
             (qFabs(P1.x() -lp->at(i).x())+qFabs(P1.y()- lp->at(i).y())) ) PG1=lp->at(i);
    }
    return PG1;
}

/*static int countfunc=0;
bool minLine(QList <netPoint*> ln1,netPoint* npadd,QList <netPoint*> *lnp,netPoint* npend,QList <netPoint*> *lnmin,qreal *lnminlen,int *minbusy)
{
    countfunc++;
    if ((countfunc>100000)){
        return false;
    }

    if (ln1.size()>150) return false;
    if (ln1.indexOf(npadd)>=0) return false;
    ln1.push_back(npadd);
    int busyCount1=busyCount(&ln1);
    if (lnmin->size()!=0){
        if (*minbusy<busyCount1)
            return  false;
        if (lnmin->size()<ln1.size())
            return false;
    }
    qreal len1=lenLine(&ln1);
    if (lnmin->size()!=0){
        if (len1>=*lnminlen)
            return false;
    }
    if (npadd==npend){
            *lnmin=ln1; *lnminlen=len1;
            *minbusy=busyCount1;
            return true;
    }
    if (ln1.size()>1){
        if (npadd->addr!=0) return false;
    }
    // выберем правильный путь!
    // где следующаяя блище к концу
    // <0 ^1 v2 >3 4 /
    QList<qreal> llen;
    QList<int> ln;
    for (int i=0;i<5;i++) {
        if (npadd->n[i]==0) continue;
        if ((ln1.size()>=2) &&(ln1[ln1.size()-2]==npadd->n[i])) continue;
        QLineF L=QLineF(npadd->n[i]->pg,npend->pg);
        qreal len=L.length();
        int ii=0;
        while (ii<ln.size()) {
            if (llen[ii]>len){
                break;
            }
            ii++;
        }
        //if (!npadd->n[i]->busy) {
            ln.insert(ii,i);
            llen.insert(ii,len);
        //}
    }






    for (int j=0;j<ln.size();j++){
        if (npadd->n[ln[j]]!=0)
            if (minLine(ln1,npadd->n[ln[j]],lnp,npend,lnmin,lnminlen,minbusy) )
                return false;
    }
    return false;
}*/

//bool v_RootLPDiag::makeLine(v_LPDiag *LP1,int addr1, int addr2, QPolygonF *LNS, QPolygonF *LNG)
//{
//    LNS->clear();LNG->clear();
//    // ищем аддрессссс
//    QList <v_LPDiag*> l=findChildren<v_LPDiag*>();
//    v_LPDiag *LP2=0;
//    for (int i=0;i<l.size();i++){
//        if (l[i]->isAddrExists(addr2)>=0){
//            LP2=l[i];break;
//        }
//    }
//    if (LP2==0) return false;
//    QList <v_LPDiag*> l2;
//    l2.push_back(LP1);l2.push_back(LP2);
//    makeNet(l2);
//    netPoint*np1=0;
//    netPoint*np2=0;
//    for (int i=0;i<lnp.size();i++){
//        if ((lnp[i]->addr==addr1)&&(lnp[i]->inout==2)) np1=lnp[i];
//        if ((lnp[i]->addr==addr2)&&(lnp[i]->inout==1)) np2=lnp[i];
//    }

//    if ((np1==0)||(np2==0)) return false;

//    QList <netPoint*> lnmin;
//    QList <netPoint*> ln1;

//    countfunc=0;
//    qreal lnminlen=0;
//    int minbusy=0;
//    minLine(ln1,np1,&lnp,np2,&lnmin,&lnminlen,&minbusy);
//    for (int i=0;i<lnmin.size();i++){
//        lnmin[i]->busy=true;
//        LNG->push_back(lnmin[i]->pg);
//        LNS->push_back(LP1->parent2point(lnmin[i]->pg,this));
//    }
//    return true;

//}

//netPoint* netPointInList(netPoint *np1,QList <netPoint*> *lnp,int n) // <0 ^1 v2 >3
//{
//    netPoint*res=0;
//    for (int i=0;i<lnp->size();i++){
//        netPoint *np2=lnp->at(i);
//        if (np1==np2)  continue;
//        if (n==0){
//            if ((np2->pg.x()<np1->pg.x()) && (np2->pg.y()==np1->pg.y())) {
//                if ((res==0) || (np2->pg.x()>res->pg.x())) res=np2;
//            }
//        }
//        if (n==3){
//            if ((np2->pg.x()>np1->pg.x()) && (np2->pg.y()==np1->pg.y())) {
//                if ((res==0) || (np2->pg.x()<res->pg.x())) res=np2;
//            }
//        }
//        if (n==1){
//            if ((np2->pg.y()<np1->pg.y()) && (np2->pg.x()==np1->pg.x())) {
//                if ((res==0) || (np2->pg.y()>res->pg.y())) res=np2;
//            }
//        }
//        if (n==2){
//            if ((np2->pg.y()>np1->pg.y()) && (np2->pg.x()==np1->pg.x())) {
//                if ((res==0) || (np2->pg.y()<res->pg.y())) res=np2;
//            }
//        }
//    }
//    return res;
//}


//void v_RootLPDiag::makeNet(QList <v_LPDiag*> l)
//{

//    if (l.size()==0) return;
//    lnp.clear();
//    for (int i=0;i<l.size();i++){
//        l[i]->addPoints2Net();
//        for (int j=0;j<l[i]->lnp.size();j++){
//            lnp.push_back(l[i]->lnp.at(j));
//        }
//    }
//    for (int i=0;i<lnp.size();i++){
//        for (int j=0;j<4;j++){
//            lnp[i]->n[j]=netPointInList(lnp[i],&lnp,j);
//        }
//    }
//    // соеденим прямых!
//    for (int i=0;i<lnp.size();i++){
//        if (lnp[i]->inout!=1) continue;
//        for (int j=0;j<lnp.size();j++){
//            if (i==j) continue;
//            if (lnp[j]->inout!=2) continue;
//            if (lnp[i]->addr==lnp[j]->addr) continue;
//            if (lnp[i]->pg.x()==lnp[j]->pg.x()) continue;
//            if (lnp[i]->pg.y()==lnp[j]->pg.y()) continue;
//            bool inters=false;
//            for (int il=0;il<l.size();il++){
//                QRectF R=l[il]->shape().boundingRect();
//                R.moveCenter(l[il]->pos());
//                if (QPFunction::isLineIntersectRect(lnp[i]->pg,lnp[j]->pg,R)){
//                    inters=true;
//                    break;
//                }
//            }
//            if (!inters) lnp[i]->n[4]=lnp[j];
//        }
//    }

//    // удалим лишних
//    for (int i=0;i<lnp.size();i++){
//        if (lnp[i]->addr!=0){
//            lnp[i]->n[1]=0;
//            lnp[i]->n[2]=0;
//        }
//        lnp[i]->busy=false;
//        for (int j=0;j<4;j++){
//            if (lnp[i]->n[j]!=0){
//                for (int il=0;il<l.size();il++){
//                    QRectF R=l[il]->shape().boundingRect();
//                    R.moveCenter(l[il]->pos());
//                    if (QPFunction::isLineIntersectRect(lnp[i]->pg,lnp[i]->n[j]->pg,R)){
//                        lnp[i]->n[j]=0;
//                        break;
//                    }
//                }

//            }
//        }
//    }


//}

void v_RootLPDiag::updateLines(int addr_krug)
{
    QList <v_LPDiag*> l=findChildren<v_LPDiag*>();
    for (int i=0;i<l.size();i++){
        if (l[i]->addr_krug!=addr_krug) continue;
        l[i]->clearLines();
    }
    for (int i=0;i<l.size();i++){
        if (l[i]->addr_krug!=addr_krug) continue;
        l[i]->makeLines();
    }
}

v_LPDiag *v_RootLPDiag::getLPDiag(int addkrug,int addr, int &n)
{
    QList <v_LPDiag*> l=findChildren<v_LPDiag*>();
    for (int i=0;i<l.size();i++){
        if (l[i]->addr_krug!=addkrug) continue;
        n=l[i]->isAddrExists(addr);
        if (n>=0){
            return l[i];
        }
    }
    return 0;
}

bool v_RootLPDiag::isIntersect(int addr_krug,QPolygonF &p)
{
    QList <v_LPDiag*> l=findChildren<v_LPDiag*>();
    for (int il=0;il<l.size();il++){
        if (l[il]->addr_krug!=addr_krug) continue;
        QRectF R=l[il]->shape().boundingRect();
        R.moveCenter(l[il]->pos());
        if (QPFunction::isPolylineIntersectRect(p,R)){
            return true;
        }
    }
    return false;
}

int v_RootLPDiag::sovpad(int addr_krug,QPolygonF &PL1)
{
    int r=0;
    QList <v_LPDiag*> l=findChildren<v_LPDiag*>();
    for (int i=0;i<l.size();i++){
        if (l[i]->addr_krug!=addr_krug) continue;
        for (int j=0;j<4;j++){
            QPolygonF PL2=l[i]->linesG[j];
            for (int t1=0;t1<PL1.size();t1++){
                for (int t2=0;t2<PL2.size()-1;t2++){
                    if (QPFunction::isPointOnLine(PL2[t2],PL2[t2+1],PL1[t1])) r++;
                }
            }
            for (int t1=0;t1<PL2.size();t1++){
                for (int t2=0;t2<PL1.size()-1;t2++){
                    if (QPFunction::isPointOnLine(PL1[t2],PL1[t2+1],PL2[t1])) r++;
                }
            }
        }
    }
    return r;
}


