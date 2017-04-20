#include <QtCore>
#include <QtWidgets>
#include "dagperf.h"

int main(int argc, char *argv[]) {
    
    QApplication app(argc, argv);  
    
    DAGPerf window(0, &app);
    
    window.resize(1400, 800);
    window.setWindowTitle("DAGPerf");
    window.setWindowIcon(QIcon("./icons/dagviz_icon.svg"));
    window.show();
    
    return app.exec();
}
