#include "cfa_cmd_export_plot.hpp"
#include <algorithm>

namespace cfa{
    void showPlot(int argc, char* argv[], std::vector<float> &timeframe, std::vector<float> &criticalpower)
    {
        std::vector<double> ts(timeframe.begin(), timeframe.end());
        std::vector<double> p(criticalpower.begin(), criticalpower.end());
        
        QApplication plotExport(argc, argv);


        // Qcustomplot only takes double for some reason...
        QVector<double> x;
        QVector<double> y;
        x = QVector<double>::fromStdVector(ts);
        y = QVector<double>::fromStdVector(p);

        double *max_power = std::max_element(y.begin(), y.end());

        
        QCustomPlot *plot = new QCustomPlot();
        
        plot->addGraph();
        plot->graph(0)->addData(x, y, true);
        plot->graph(0)->setPen(QPen(Qt::blue));
        plot->xAxis->setRange(0,3600);
        plot->yAxis->setRange(0,*max_power + 50);
        plot->xAxis->setLabel("OHNWWWWWWWWWWWWWWW");
        
        std::string fullPath = std::string(argv[2])+"/power_curve.png";

        plot->savePng(fullPath.c_str(), 400, 300);
        
        delete plot;

        return plotExport.exit(0);
    }

}