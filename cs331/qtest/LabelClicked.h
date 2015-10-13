
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QFont>
#include <QDebug>
class LabelClicked : public QLabel
{
	Q_OBJECT
	public:
		LabelClicked(const QString &text, QWidget *parent = 0);
		~LabelClicked(){}
		int id;
	signals:
		void clicked();
	public slots:
		void slotClicked();
	protected:
		void mousePressEvent(QMouseEvent *event);
};
