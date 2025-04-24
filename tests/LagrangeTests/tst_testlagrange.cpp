#include <QtTest>

#include "B:\TechProg Git\TechProg-Gray-241-327-(1)\serverfunctions.h"

class TestLagrange : public QObject
{
    Q_OBJECT

private slots:
    void test_3_points();
    void test_4_points();
    void test_5_points();
    void test_duplicate_x();
    void test_edge_case();
};

void TestLagrange::test_3_points()
{
    QVector<double> x = {1, 2, 3};
    QVector<double> y = {1, 4, 9}; // y = x^2
    double point = 2.5;
    double expected = 6.25;
    QCOMPARE(lagrangeInterpolation(x, y, point), expected);
}

void TestLagrange::test_4_points()
{
    QVector<double> x = {0, 1, 2, 3};
    QVector<double> y = {0, 1, 8, 27}; // y = x^3
    double point = 1.5;
    double expected = 3.375;
    QCOMPARE(lagrangeInterpolation(x, y, point), expected);
}

void TestLagrange::test_5_points()
{
    QVector<double> x = {-2, -1, 0, 1, 2};
    QVector<double> y = {4, 1, 0, 1, 4}; // y = x^2
    double point = 0.5;
    double expected = 0.25;
    QCOMPARE(lagrangeInterpolation(x, y, point), expected);
}

void TestLagrange::test_duplicate_x()
{
    QVector<double> x = {1, 1, 2}; // Дубликат x=1
    QVector<double> y = {1, 2, 3};
    QVERIFY_THROWS_EXCEPTION(std::invalid_argument, lagrangeInterpolation(x, y, 1.5));
}

void TestLagrange::test_edge_case()
{
    QVector<double> x = {0};
    QVector<double> y = {5};
    QCOMPARE(lagrangeInterpolation(x, y, 0), 5.0);
}

QTEST_APPLESS_MAIN(TestLagrange)
#include "tst_testlagrange.moc"
