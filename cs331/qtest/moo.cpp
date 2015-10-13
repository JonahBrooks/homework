#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QFont>
#include <QDebug>
#include "LabelClicked.h"
#include "LabelWidget.h"
#include <vector>

bool isP1sTurn = true;

LabelClicked::LabelClicked(const QString &text, QWidget *parent)
	:QLabel(text,parent)
{
	connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
}
void LabelClicked::mousePressEvent(QMouseEvent *event)
{
	emit clicked();
}
void LabelClicked::slotClicked()
{
	qDebug()<<"Clicked() " << id;
	if(text() == "_"){
		if(isP1sTurn ) {
			setText("X");
			isP1sTurn = ! isP1sTurn;
		}
		else {
			setText("O");
			isP1sTurn = ! isP1sTurn;
		}
	}
}


LabelWidget::LabelWidget(std::vector<LabelClicked*> labels,QWidget *parent)
	: QWidget(parent)
{
	QString token = "_";
	QString divider="|";
	std::vector<LabelClicked*> dividers;
	
	int x = 0;
	int y = 60;
	int index = 0;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			labels.push_back(new LabelClicked(token,this));
			labels[index]->move(x,y);
			dividers.push_back(new LabelClicked(divider,this));
			dividers[index]->move(x+10,y);
			x += 20;
			labels[index]->id = index;
			index++;
		}
		y -= 20;
		x = 0;
	}


}

int main(int argc, char *argv[])
{
	QApplication app(argc,argv);

	std::vector<LabelClicked*> labels;
	LabelWidget window(labels);

	window.resize(60,80);
	window.setWindowTitle("Simple Example");
	window.show();

	return app.exec();
}
