
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QFont>
#include <QDebug>
class LabelWidget : public QWidget
{
	public:
		LabelWidget(std::vector<LabelClicked*>, QWidget *parent = 0);

	private:
		LabelClicked *label;
};
