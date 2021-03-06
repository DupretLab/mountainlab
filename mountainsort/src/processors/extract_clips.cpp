#include "extract_clips.h"
#ifdef USE_LAPACK
#include "get_pca_features.h"
#else
#include "get_principal_components.h"
#endif
#include "get_principal_components.h"

Mda extract_clips(const DiskReadMda& X, const QVector<double>& times, int clip_size)
{
    int M = X.N1();
    int N = X.N2();
    int T = clip_size;
    int L = times.count();
    int Tmid = (int)((T + 1) / 2) - 1;
    Mda clips(M, T, L);
    for (int i = 0; i < L; i++) {
        int t1 = (int)times[i] - Tmid;
        int t2 = t1 + T - 1;
        if ((t1 >= 0) && (t2 < N)) {
            Mda tmp;
            X.readChunk(tmp, 0, t1, M, T);
            for (int t = 0; t < T; t++) {
                for (int m = 0; m < M; m++) {
                    clips.set(tmp.get(m, t), m, t, i);
                }
            }
        }
    }
    return clips;
}

Mda32 extract_clips(const DiskReadMda32& X, const QVector<double>& times, int clip_size)
{
    int M = X.N1();
    int N = X.N2();
    int T = clip_size;
    int L = times.count();
    int Tmid = (int)((T + 1) / 2) - 1;
    Mda32 clips(M, T, L);
    for (int i = 0; i < L; i++) {
        int t1 = (int)times[i] - Tmid;
        int t2 = t1 + T - 1;
        if ((t1 >= 0) && (t2 < N)) {
            Mda32 tmp;
            X.readChunk(tmp, 0, t1, M, T);
            for (int t = 0; t < T; t++) {
                for (int m = 0; m < M; m++) {
                    clips.set(tmp.get(m, t), m, t, i);
                }
            }
        }
    }
    return clips;
}

Mda extract_clips(const DiskReadMda& X, const QVector<double>& times, const QVector<int>& channels, int clip_size)
{
    int M = X.N1();
    int N = X.N2();
    int M0 = channels.count();
    int T = clip_size;
    int L = times.count();
    int Tmid = (int)((T + 1) / 2) - 1;
    Mda clips(M0, T, L);
    for (int i = 0; i < L; i++) {
        int t1 = (int)times[i] - Tmid;
        int t2 = t1 + T - 1;
        if ((t1 >= 0) && (t2 < N)) {
            Mda tmp;
            X.readChunk(tmp, 0, t1, M, T);
            for (int t = 0; t < T; t++) {
                for (int m0 = 0; m0 < M0; m0++) {
                    clips.set(tmp.get(channels[m0], t), m0, t, i);
                }
            }
        }
    }
    return clips;
}

Mda32 extract_clips(const DiskReadMda32& X, const QVector<double>& times, const QVector<int>& channels, int clip_size)
{
    int M = X.N1();
    int N = X.N2();
    int M0 = channels.count();
    int T = clip_size;
    int L = times.count();
    int Tmid = (int)((T + 1) / 2) - 1;
    qDebug() << "####################" << M << N << M0 << T << L;
    Mda32 clips(M0, T, L);
    for (int i = 0; i < L; i++) {
        int t1 = (int)times[i] - Tmid;
        int t2 = t1 + T - 1;
        if ((t1 >= 0) && (t2 < N)) {
            Mda32 tmp;
            X.readChunk(tmp, 0, t1, M, T);
            for (int t = 0; t < T; t++) {
                for (int m0 = 0; m0 < M0; m0++) {
                    clips.set(tmp.get(channels[m0], t), m0, t, i);
                }
            }
        }
    }
    return clips;
}

bool extract_clips(const QString& timeseries_path, const QString& firings_path, const QString& clips_path, int clip_size, const QList<int> &channels_in,double t1,double t2)
{
    qDebug() << "++++++++++++++++++++++++++++";
    QVector<int> channels;
    for (int i=0; i<channels_in.count(); i++)
        channels << channels_in[i]-1;
    DiskReadMda X(timeseries_path);
    DiskReadMda F(firings_path);
    QVector<double> times;
    for (long j = 0; j < F.N2(); j++) {
        double t0=F.value(1,j);
        if ((t2==0)||((t1<=t0)&&(t0<=t2)))
            times << F.value(1, j);
    }
    qDebug() << __FILE__ << __LINE__ << times.count();
    Mda clips = extract_clips(X, times, channels, clip_size);
    clips.write32(clips_path);
    return true;
}

/*
bool extract_clips_features(const QString& timeseries_path, const QString& firings_path, const QString& features_path, int clip_size, int num_features)
{
    DiskReadMda X(timeseries_path);
    DiskReadMda F(firings_path);
    QVector<double> times;
    for (long j = 0; j < F.N2(); j++) {
        times << F.value(1, j);
    }
    Mda clips = extract_clips(X, times, clip_size);
    Mda features(num_features, clips.N3());
    get_pca_features_2(clips.N1() * clips.N2(), clips.N3(), num_features, features.dataPtr(), clips.dataPtr());
    features.write32(features_path);
    return true;
}
*/
