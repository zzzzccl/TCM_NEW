#include "pim_tcm.h"

namespace pim_tcm
{

void DMA_FE::ProcessDMAWr0(void)
{
    IF_GEN::if_dma_tcm_wr_req req_in = {};

    while(true)
    {
        while(!m_dma_tcm_wr0_req_if.num_available())
        {
            wait();
        }

        m_dma_tcm_wr0_req_if.nb_read(req_in);
        Sideband sb;
        Payload pl;
        Request req_out;
        if (req_in.op == 2){
            sb.addr = TCMAddress(req_in.write_sem_union.sem.addr);
            sb.sem_num = req_in.write_sem_union.sem.sem_num;
        }
        else{
            sb.addr = TCMAddress(req_in.write_sem_union.write.addr);
            sb.instr_id = req_in.write_sem_union.instr_id;
            sb.instr_last = req_in.write_sem_union.write.instr_last;
            sb.resp_type = req_in.op;

            for(uint32_t u = 0; u < 32; u++)
            {
                pl.data[u] = req_in.write_sem_union.write.data[u];
            }

            for(uint32_t u = 0; u < 4; u++)
            {
                pl.mask[u] = req_in.write_sem_union.write.mask[u];
            }

            uint32_t lane_idx = sb.addr.lane_index;
            req_out.mask[lane_idx] = 1;
        }

        req_out.sideband = sb;
        req_out.payload = pl;



        m_dma_wr0_out.write(req_out);
    }
}

void DMA_FE::ProcessDMAWr1(void)
{
    IF_GEN::if_dma_tcm_wr_req req_in = {};

    while(true)
    {
        while(!m_dma_tcm_wr1_req_if.num_available())
        {
            wait();
        }

        m_dma_tcm_wr1_req_if.nb_read(req_in);
        Sideband sb;
        Payload pl;
        Request req_out;
        if (req_in.op == 2){
            sb.addr = TCMAddress(req_in.write_sem_union.sem.addr);
            sb.sem_num = req_in.write_sem_union.sem.sem_num;
        }
        else{
            sb.addr = TCMAddress(req_in.write_sem_union.write.addr);
            sb.instr_id = req_in.write_sem_union.instr_id;
            sb.instr_last = req_in.write_sem_union.write.instr_last;
            sb.resp_type = req_in.op;

            for(uint32_t u = 0; u < 32; u++)
            {
                pl.data[u] = req_in.write_sem_union.write.data[u];
            }

            for(uint32_t u = 0; u < 4; u++)
            {
                pl.mask[u] = req_in.write_sem_union.write.mask[u];
            }

            uint32_t lane_idx = sb.addr.lane_index;
            req_out.mask[lane_idx] = 1;
        }

        req_out.sideband = sb;
        req_out.payload = pl;



        m_dma_wr1_out.write(req_out);
    }
}

void DMA_FE::ProcessDMARdData(void)
{
    IF_GEN::if_dma_tcm_rd_data_req req_in = {};

    while(true)
    {
        while(!m_dma_tcm_rd_data_req_if.num_available())
        {
            wait();
        }

        m_dma_tcm_rd_data_req_if.nb_read(req_in);
        Sideband sb;
        Payload pl;
        Request req_out;
        sb.addr = TCMAddress(req_in.addr);
        sb.req_id = req_in.req_id;
        sb.burst_len = req_in.burst_len;

        req_out.sideband = sb;
        uint8_t lane_idx = sb.addr.lane_index;
        for (uint32_t u = 0; u <= sb.burst_len; u++){
            req_out.mask[lane_idx + u] = 1;
        }

        m_dma_wr0_out.write(req_out);
    }
}

void DMA_FE::ProcessDMARdDesc(void)
{
    IF_GEN::if_dma_tcm_rd_desc_req req_in = {};

    while(true)
    {
        while(!m_dma_tcm_rd_desc_req_if.num_available())
        {
            wait();
        }

        m_dma_tcm_rd_desc_req_if.nb_read(req_in);
        Sideband sb;
        Payload pl;
        Request req_out;
        sb.addr = TCMAddress(req_in.addr);
        sb.cl_id = req_in.cl_id;
        sb.burst_len = 3;
        for (uint32_t u = 0; u <= sb.burst_len; u++){
            sb.blk_id = u;
            req_out.sideband = sb;
            m_dma_wr0_out.write(req_out);
        }
    }
}

void TC_FE::ProcessTCWr(void)
{
    IF_GEN::if_tc_tcm_wr req_in = {};

    while(true)
    {
        while(!m_tc_tcm_wr_if.num_available())
        {
            wait();
        }

        m_tc_tcm_wr_if.nb_read(req_in);
        Sideband sb;
        Payload pl;
        Request req_out;
        if (req_in.op == 2){
            sb.addr = TCMAddress(req_in.write_sem_union.sem.addr);
            sb.sem_num = req_in.write_sem_union.sem.sem_num;
            sb.instr_last = req_in.write_sem_union.sem.instr_last;
        }
        else if (req_in.op == 1){
            sb.addr = TCMAddress(req_in.write_sem_union.write.addr);
            sb.op = req_in.op;
            sb.rvs_core_id = req_in.rvs_core_id;
            sb.instr_id = req_in.instr_id;

            for(uint32_t u = 0; u < 32; u++)
            {
                pl.data[u] = req_in.write_sem_union.write.data[u];
            }

            for(uint32_t u = 0; u < 4; u++)
            {
                pl.mask[u] = 0xFFFFFFFF;
            }

            uint32_t lane_idx = sb.addr.lane_index;
            req_out.mask[lane_idx] = 1;
        }

        req_out.sideband = sb;
        req_out.payload = pl;

        m_tc_wr_out.write(req_out);
    }
}

void TC_FE::ProcessTCRd0(void)
{
    IF_GEN::if_tc_tcm_b_rd req_in = {};

    while(true)
    {
        while(!m_tc_tcm_b_rd_if.num_available())
        {
            wait();
        }

        m_tc_tcm_b_rd_if.nb_read(req_in);
        Sideband sb;
        Payload pl;
        Request req_out;
        sb.addr = TCMAddress(req_in.addr);
        sb.rvs_core_id = req_in.rvs_core_id;
        sb.instr_id = req_in.instr_id;
        sb.buf_inx = req_in.buf_inx;
        sb.burst_len = req_in.burst_len;

        uint32_t lane_idx = sb.addr.lane_index;
        req_out.mask[lane_idx] = 1;

        req_out.sideband = sb;

        m_tc_rd0_out.write(req_out);
    }
}

void TC_FE::ProcessTCRd1(void)
{
    IF_GEN::if_tc_tcm_ac_sf_rd req_in = {};

    while(true)
    {
        while(!m_tc_tcm_ac_sf_rd_if.num_available())
        {
            wait();
        }

        m_tc_tcm_ac_sf_rd_if.nb_read(req_in);
        Sideband sb;
        Payload pl;
        Request req_out;
        sb.rvs_core_id = req_in.rvs_core_id;
        sb.instr_id = req_in.instr_id;
        sb.buf_inx = req_in.buf_inx;
        sb.matrix_src = req_in.matrix_src;
        if (sb.matrix_src == 0){
            sb.addr = TCMAddress(req_in.SrcInfoUnion.matrix_a.addr);
            sb.grp_mask = req_in.SrcInfoUnion.matrix_a.grp_mask;
        }
        else if (sb.matrix_src == 1){
            sb.addr = TCMAddress(req_in.SrcInfoUnion.matrix_c.addr);
            sb.grp_mask = req_in.SrcInfoUnion.matrix_c.grp_mask;
        }
        else if (sb.matrix_src == 2){
            sb.addr = TCMAddress(req_in.SrcInfoUnion.matrix_sa.addr);
            sb.grp_mask = req_in.SrcInfoUnion.matrix_sa.grp_mask;
        }
        else if (sb.matrix_src == 3){
            sb.addr = TCMAddress(req_in.SrcInfoUnion.matrix_sb.addr);
            sb.grp_mask = req_in.SrcInfoUnion.matrix_sb.grp_mask;
        }

        uint32_t lane_idx = sb.addr.lane_index;
        req_out.mask[lane_idx] = 1;

        req_out.sideband = sb;

        m_tc_rd1_out.write(req_out);
    }
}

void RVV_FE::ProcessVLSUWr(void)
{
    IF_GEN::if_vlsu_tcm_wr req_in = {};

    while(true)
    {
        while(!m_vlsu0_tcm_wr_if.num_available() && !m_vlsu1_tcm_wr_if.num_available() && !m_vlsu2_tcm_wr_if.num_available() && !m_vlsu3_tcm_wr_if.num_available())
        {
            wait();
        }
        if (m_vlsu0_tcm_wr_if.num_available()){
            m_vlsu0_tcm_wr_if.nb_read(req_in);
        }
        else if (m_vlsu1_tcm_wr_if.num_available()){
            m_vlsu1_tcm_wr_if.nb_read(req_in);
        }
        else if (m_vlsu2_tcm_wr_if.num_available()){
            m_vlsu2_tcm_wr_if.nb_read(req_in);
        }
        else if (m_vlsu3_tcm_wr_if.num_available()){
            m_vlsu3_tcm_wr_if.nb_read(req_in);
        }

        Sideband sb;
        Payload pl;
        Request req_out;
        uint8_t isSem = 0;
        sb.op = req_in.op;
        if (req_in.op == 2 && req_in.wr_union.vmem_fence.sem_post_en == 1){
            sb.addr = TCMAddress(req_in.wr_union.vmem_fence.addr);
            isSem = 1;
        }
        else if (req_in.op == 1){
            sb.addr = TCMAddress(req_in.wr_union.write.addr);

            for(uint32_t u = 0; u < 32; u++)
            {
                pl.data[u] = req_in.wr_union.write.data[u];
            }

            for(uint32_t u = 0; u < 4; u++)
            {
                pl.mask[u] = 0xFFFFFFFF;
            }

            uint32_t lane_idx = sb.addr.lane_index;
            req_out.mask[lane_idx] = 1;
        }

        req_out.sideband = sb;
        req_out.payload = pl;

        if (m_vlsu0_tcm_wr_if.num_available()){
            m_vlsu0_wr_out.write(req_out);
            if (isSem){
                m_vlsu0_sem_out.write(req_out);
            }
        }
        else if (m_vlsu1_tcm_wr_if.num_available()){
            m_vlsu1_wr_out.write(req_out);
            if (isSem){
                m_vlsu1_sem_out.write(req_out);
            }
        }
        else if (m_vlsu2_tcm_wr_if.num_available()){
            m_vlsu2_wr_out.write(req_out);
            if (isSem){
                m_vlsu2_sem_out.write(req_out);
            }
        }
        else if (m_vlsu3_tcm_wr_if.num_available()){
            m_vlsu3_wr_out.write(req_out);
            if (isSem){
                m_vlsu3_sem_out.write(req_out);
            }
        }

        m_tc_wr_out.write(req_out);
    }
}

void RVV_FE::ProcessVLSURd(void)
{
    IF_GEN::if_vlsu_tcm_rd req_in = {};

    while(true)
    {
        while(!m_vlsu0_tcm_wr_if.num_available() && !m_vlsu1_tcm_wr_if.num_available() && !m_vlsu2_tcm_wr_if.num_available() && !m_vlsu3_tcm_wr_if.num_available())
        {
            wait();
        }
        if (m_vlsu0_tcm_wr_if.num_available()){
            m_vlsu0_tcm_wr_if.nb_read(req_in);
        }
        else if (m_vlsu1_tcm_wr_if.num_available()){
            m_vlsu1_tcm_wr_if.nb_read(req_in);
        }
        else if (m_vlsu2_tcm_wr_if.num_available()){
            m_vlsu2_tcm_wr_if.nb_read(req_in);
        }
        else if (m_vlsu3_tcm_wr_if.num_available()){
            m_vlsu3_tcm_wr_if.nb_read(req_in);
        }

        m_tc_tcm_ac_sf_rd_if.nb_read(req_in);
        Sideband sb;
        Payload pl;
        Request req_out;
        sb.rvs_core_id = req_in.rvs_core_id;
        sb.instr_id = req_in.instr_id;
        sb.buf_inx = req_in.buf_inx;
        sb.matrix_src = req_in.matrix_src;
        if (sb.matrix_src == 0){
            sb.addr = TCMAddress(req_in.SrcInfoUnion.matrix_a.addr);
            sb.grp_mask = req_in.SrcInfoUnion.matrix_a.grp_mask;
        }
        else if (sb.matrix_src == 1){
            sb.addr = TCMAddress(req_in.SrcInfoUnion.matrix_c.addr);
            sb.grp_mask = req_in.SrcInfoUnion.matrix_c.grp_mask;
        }
        else if (sb.matrix_src == 2){
            sb.addr = TCMAddress(req_in.SrcInfoUnion.matrix_sa.addr);
            sb.grp_mask = req_in.SrcInfoUnion.matrix_sa.grp_mask;
        }
        else if (sb.matrix_src == 3){
            sb.addr = TCMAddress(req_in.SrcInfoUnion.matrix_sb.addr);
            sb.grp_mask = req_in.SrcInfoUnion.matrix_sb.grp_mask;
        }

        uint32_t lane_idx = sb.addr.lane_index;
        req_out.mask[lane_idx] = 1;

        req_out.sideband = sb;

        m_tc_rd1_out.write(req_out);
    }
}

}